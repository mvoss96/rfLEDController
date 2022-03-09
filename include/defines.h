#pragma once
#define DEVICE_NAME "rfLEDController"

/*Control Settings*/ 
//#define RF_CONTROLL     //needs a connected nrf24l01
#define WIFI_CONTROLL     //only on ESP


/*single(1), dual Color(2) or RGB(3)*/
#define LED_NUM 1 
#define MIN_BR 12

/*set up LED pins, -1 for disable*/
#define LED1_PIN 3
#define LED2_PIN -1
#define LED3_PIN -1

/*set up NRF24l01, -1 for disable */
#define RADIO_ID 42
#define PIN_RADIO_CE 10
#define PIN_RADIO_CSN 9
#define RADIO_CHANNEL 100