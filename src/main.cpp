#include "defines.h"
#include <Arduino.h>
#include "leds.h"

void rfSetup();
void rfListen();
void wifiSetup();
void wifiListen();

Leds Led(LED1_PIN, LED2_PIN, LED3_PIN);

void setup()
{
  Serial.begin(115200);
  while (!Serial) // wait for connection
    ;
  delay(200); 
  Serial.print(F("\nStarting up "));
  Serial.println(DEVICE_NAME);
#ifdef WIFI_CONTROLL
  wifiSetup();
#endif
#ifdef RF_CONTROLL
  rfSetup();
#endif
  Led.setLB();
}

void loop()
{
#ifdef RF_CONTROLL
  rfListen();
#endif
#ifdef WIFI_CONTROLL
  wifiListen();
#endif
}