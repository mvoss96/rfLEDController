#include "defines.h"
#include <Arduino.h>
#include "leds.h"

void rfSetup();
void rfListen();

Leds Led(LED1_PIN, LED2_PIN, LED3_PIN);

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
  delay(200); //wait for serial connection
  Serial.print(F("\nStarting up "));
  Serial.println(PROJECT_NAME);
#ifdef WIFI_CONTROLL
  WifiSetup();
#endif
#ifdef RF_CONTROLL
  rfSetup();
#endif
}

void loop()
{
#ifdef RF_CONTROLL
  rfListen();
#endif
}