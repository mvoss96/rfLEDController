#include "defines.h"
#ifdef WIFI_CONTROLL

#define USE_LITTLEFS true
#define ESP_DRD_USE_LITTLEFS true
#define DRD_TIMEOUT 10
#define DRD_ADDRESS 0
#define REQUIRE_ONE_SET_SSID_PW true

#include <ESPAsync_WiFiManager.h>
#include <DNSServer.h>
#include <ESP_DoubleResetDetector.h>
#include <AsyncElegantOTA.h>

void wifiSetup()
{
}

void wifiListen()
{
}

#endif