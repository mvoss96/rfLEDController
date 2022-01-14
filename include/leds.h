#pragma once

class Leds
{
private:
    int c1Pin = -1, c2Pin = -1, c3Pin = -1;

public:
    Leds(uint8_t c1, uint8_t c2, uint8_t c3)
    {
        c1Pin = c1;
        c2Pin = c2;
        c3Pin = c3;
        pinMode(c1Pin, OUTPUT);
        pinMode(c2Pin, OUTPUT);
        pinMode(c3Pin, OUTPUT);
    }

    void br(uint8_t b1)
    {
        analogWrite(c1Pin, b1);
    }

    void cTemp(uint8_t b1, uint8_t b2)
    {
        analogWrite(c1Pin, b1);
        analogWrite(c2Pin, b2);
    }

    void rgb(uint8_t b1, uint8_t b2, uint8_t b3)
    {
        analogWrite(c1Pin, b1);
        analogWrite(c2Pin, b2);
        analogWrite(c3Pin, b3);
    }
};