#include "defines.h"
#include <SPI.h>
#include <math.h>
#include <NRFLite.h>
#include "leds.h"

//RF Message Types
#define TYPE_POWER 0
#define TYPE_BR_SET 1
#define TYPE_BR_DEC 2
#define TYPE_BR_INC 3
#define TYPE_CO_SET 4
#define TYPE_CO_DEC 5
#define TYPE_CO_INC 6
#define TYPE_PRESET 7
#define TYPE_RGB 8
#define TYPE_ACK_DATA 255

extern Leds Led;

struct __attribute__((packed)) RadioPacket //Any packet up to 32 bytes can be sent.
{
    uint8_t FromRadioId;
    uint8_t msgType;
    uint8_t value;
    byte extra[2];
};
NRFLite _radio;
RadioPacket _radioData;

void printRF(RadioPacket &pck)
{
    String msg = "-> id: ";
    msg += pck.FromRadioId;
    msg += ", type: ";
    msg += pck.msgType;
    msg += ", value: ";
    msg += pck.value;
    msg += ", extra: ";
    msg += (int)pck.extra[0];
    msg += " ";
    msg += (int)pck.extra[1];
    Serial.println(msg);
}

void rfSetup()
{
    while (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE250KBPS, 100))
    {
        Serial.println("Cannot communicate with radio");
        delay(1000);
    }
}

int parseRF(RadioPacket &pck)
{
    switch (pck.msgType)
    {
    case TYPE_POWER:
    {
        if (pck.value == 0)
            Led.setOn(false);
        else if (pck.value == 1)
            Led.setOn(true);
        else if (pck.value == 2)
            Led.toggleOn();
        else
            return -1;
        return 0;
    }
    case TYPE_BR_SET:
    {
        Led.setBr(pck.value);
        return 0;
    }
    case TYPE_BR_DEC:
    {
        Led.decBr(pck.value);
        return 0;
    }
    case TYPE_BR_INC:
    {
        Led.incBr(pck.value);
        return 0;
    }
    case TYPE_CO_SET:
    {
        Led.setCo(pck.value);
        return 0;
    }
    case TYPE_CO_DEC:
    {
        Led.decCo(pck.value);
        return 0;
    }
    case TYPE_CO_INC:
    {
        Led.incCo(pck.value);
        return 0;
    }
    case TYPE_PRESET:
    {
        Led.setCo(127);
        return 0;
    }
    case TYPE_ACK_DATA:
    {
        return 0;
    }
    default:
    {
        return -1;
    }
    }
}

void rfListen()
{
    while (_radio.hasData())
    {
        _radio.readData(&_radioData); // Note how '&' must be placed in front of the variable name.
        printRF(_radioData);
        if (parseRF(_radioData) == -1)
            Serial.println("Error parsing previous packet");
        RadioPacket ackData;
        ackData.FromRadioId = RADIO_ID;
        ackData.msgType = TYPE_ACK_DATA;
        ackData.value = Led.getC1();
        ackData.extra[0] = Led.getC2();
        ackData.extra[1] = Led.getC3();
        _radio.addAckData(&ackData, sizeof(ackData));
    }
}