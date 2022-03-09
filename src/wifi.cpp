#include "defines.h"
#ifdef WIFI_CONTROLL

#define USE_LITTLEFS true
#define ESP_DRD_USE_LITTLEFS true
#define DRD_TIMEOUT 10
#define DRD_ADDRESS 0
#define REQUIRE_ONE_SET_SSID_PW true

#include "leds.h"
#include <ESPAsync_WiFiManager.h>
#include <DNSServer.h>
#include <ESP_DoubleResetDetector.h>
#include <AsyncElegantOTA.h>

DoubleResetDetector drd(DRD_TIMEOUT, DRD_ADDRESS);
AsyncWebServer webServer(80);
DNSServer dnsServer;
bool initialConfig = false;
extern Leds Led;

void checkReset()
{
    if (drd.detectDoubleReset())
    {
        Serial.println(F("DRD"));
        initialConfig = true;
    }
    if (initialConfig)
    {
        Serial.println(F("Starting Config Portal"));
        ESPAsync_WiFiManager ESPAsync_wifiManager(&webServer, &dnsServer, DEVICE_NAME);
        ESPAsync_wifiManager.setConfigPortalTimeout(120);
        if (!ESPAsync_wifiManager.startConfigPortal(DEVICE_NAME))
        {
            Serial.println(F("Not connected to WiFi"));
        }
        else
        {
            Serial.println(F("connected"));
        }
    }
}

void wifiListen()
{
    drd.loop(); // doubleReset detection background loop
}

bool handleBodyPower(AsyncWebServerRequest *request, uint8_t *data, size_t len)
{
    Serial.printf("[REQUEST POWER]\t%.*s\r\n", len, (const char *)data);
    if (memcmp(data, "off", 3) == 0 || memcmp(data, "OFF", 3) == 0)
    {
        Led.setOn(false);
        return 1;
    }
    else if (memcmp(data, "on", 2) == 0 || memcmp(data, "ON", 2) == 0)
    {
        Led.setOn(true);
        return 1;
    }
    else
    {
        return 0;
    }
}

bool handleBodyBrightness(AsyncWebServerRequest *request, uint8_t *data, size_t len)
{
    data[len] = '\0';
    Serial.printf("[REQUEST BRIGHTNESS]\t%.*s\r\n", len, (const char *)data);
    int val = atoi((char *)data);
    Serial.printf("parsed: %d\n", val);
    if (val >= 0 && val <= 255)
    {
        if (val < MIN_BR)
            val = MIN_BR;
        Led.setBr(val);
        return 1;
    }
    else
    {
        return 0;
    }
}

bool handleBodyColor(AsyncWebServerRequest *request, uint8_t *data, size_t len)
{
    if (Led.getDwMode())
        return 0;
    data[len] = '\0';
    Serial.printf("[REQUEST COLOR]\t%.*s\r\n", len, (const char *)data);
    int val = atoi((char *)data);
    if (val >= 0 && val <= 255)
    {
        Led.setCo(val);
        return 1;
    }
    else
    {
        return 0;
    }
}

bool handleBodyColorMired(AsyncWebServerRequest *request, uint8_t *data, size_t len)
{
    if (Led.getDwMode())
        return 0;
    data[len] = '\0';
    Serial.printf("[REQUEST COLOR-MIRED]\t%.*s\r\n", len, (const char *)data);
    uint8_t val = atoi((char *)data);
    if (val >= 50 && val <= 400)
    {
        val = round((val - 50) / (350 / 255.0));
        Led.setCo(val);
        return 1;
    }
    else
    {
        return 0;
    }
}

bool handleBodyRGB(AsyncWebServerRequest *request, uint8_t *data, size_t len)
{
    data[len] = '\0';
    Serial.printf("[REQUEST RGB]\t%.*s\r\n", len, (const char *)data);
    char delim[] = ":;, ";
    char *ptr = strtok((char *)data, delim);
    if (ptr == NULL)
        return 0;
    uint8_t valR = atoi(ptr);
    ptr = strtok(NULL, delim);
    if (ptr == NULL)
        return 0;
    uint8_t valG = atoi(ptr);
    ptr = strtok(NULL, delim);
    if (ptr == NULL)
        return 0;
    uint8_t valB = atoi(ptr);
    Led.lc(valR, valG, valB);
    return 1;
}

void startServer()
{
    webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->redirect("/update"); });
    webServer.on("/power", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->send(200, "text/plain", (Led.getOn()) ? "on" : "off"); });
    webServer.on("/brightness", HTTP_GET, [](AsyncWebServerRequest *request)
                 {
    char msg[4];
    sprintf(msg, "%d", Led.getBr());
    request->send(200, "text/plain", msg); });
    webServer.on("/color", HTTP_GET, [](AsyncWebServerRequest *request)
                 {
    char msg[4];
    if (Led.getDwMode()) {
      sprintf(msg, "%d", Led.getCo());
      request->send(200, "text/plain", msg);
    }
    else {
      request->send(400, "text/plain", "not supported");
    } });

    webServer.on("/color-mired", HTTP_GET, [](AsyncWebServerRequest *request)
                 {
    char msg[4];
    if (Led.getDwMode()) {
      sprintf(msg, "%d", (int)(50 + (350/255.0) * Led.getCo()));
      request->send(200, "text/plain", msg);
    }
    else {
      request->send(400, "text/plain", "not supported");
    } });

    webServer.on("/rgb", HTTP_GET, [](AsyncWebServerRequest *request)
                 {
    char msg[15];
    sprintf(msg, "%d:%d:%d", Led.getC1(),Led.getC2(),Led.getC3());
    request->send(200, "text/plain", msg); });

    webServer.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
                            {
                                if (request->url() == "/power")
                                {
                                    if (!handleBodyPower(request, data, len))
                                        request->send(400, "text/plain", "");
                                    request->send(200, "text/plain", "ok");
                                }
                                else if (request->url() == "/brightness")
                                {
                                    if (!handleBodyBrightness(request, data, len))
                                        request->send(400, "text/plain", "");
                                    request->send(200, "text/plain", "ok");
                                }
                                else if (request->url() == "/color")
                                {
                                    if (!handleBodyColor(request, data, len))
                                        request->send(400, "text/plain", "");
                                    request->send(200, "text/plain", "ok");
                                }
                                else if (request->url() == "/color-mired")
                                {
                                    if (!handleBodyColorMired(request, data, len))
                                        request->send(400, "text/plain", "");
                                    request->send(200, "text/plain", "ok");
                                }
                                else if (request->url() == "/rgb")
                                {
                                    if (!handleBodyRGB(request, data, len))
                                        request->send(400, "text/plain", "");
                                    request->send(200, "text/plain", "ok");
                                } });

    AsyncElegantOTA.begin(&webServer); // Start ElegantOTA
    webServer.begin();
    Serial.println("OTA-server started");
}

void wifiSetup()
{
    WiFi.persistent(true);
    Serial.println(ARDUINO_BOARD);
    Serial.println(ESP_ASYNC_WIFIMANAGER_VERSION);
    if (WiFi.SSID() == "")
    {
        Serial.println(F("No AP credentials"));
        initialConfig = true; // no config: need for setup
    }
    checkReset();
    delay(100);
    WiFi.mode(WIFI_STA);
    if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.reconnect();
    }
    unsigned long startedAt = millis();
    Serial.print(F("After waiting "));
    int connRes = WiFi.waitForConnectResult();
    float waited = (millis() - startedAt);
    Serial.print(waited / 1000);
    Serial.print(F(" secs , Connection result is "));
    Serial.println(connRes);
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println(F("Failed to connect"));
        delay(3000);
        ESP.restart();
    }
    else
    {
        Serial.print(F("Local IP: "));
        Serial.println(WiFi.localIP());
    }
    startServer();
}

#endif