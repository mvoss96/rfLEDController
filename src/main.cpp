#include "defines.h"
#include <Arduino.h>
#include "leds.h"

Leds Led(LED1_PIN, LED2_PIN, LED3_PIN);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial)
    ;
  delay(200); //wait for serial connection
  Serial.print(F("\nStarting up "));
  Serial.println(PROJECT_NAME);
}

void loop()
{
  // put your main code here, to run repeatedly:
}