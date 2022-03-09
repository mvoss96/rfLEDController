#pragma once
#include "Arduino.h"
class Leds
{
private:
    bool on = true;
    bool dualWhite = false;
    uint8_t brightness = 127, dwColor = 127;
    uint8_t c1Pin = -1, c2Pin = -1, c3Pin = -1;
    uint8_t c1 = 0, c2 = 0, c3 = 0;

public:
    Leds(uint8_t c1, uint8_t c2, uint8_t c3)
    {
        c1Pin = c1;
        c2Pin = c2;
        c3Pin = c3;
        this->c1 = (c1Pin != -1) ? 255 : 0;
        this->c2 = (c2Pin != -1) ? 255 : 0;
        this->c3 = (c3Pin != -1) ? 255 : 0;
        dualWhite = (c1Pin != -1 && c2Pin != -1 && c3Pin == -1);
        pinMode(c1Pin, OUTPUT);
        pinMode(c2Pin, OUTPUT);
        pinMode(c3Pin, OUTPUT);
    }

    void setLB()
    {
        //Serial.println("setLB");
        if (on)
        {
            if (dualWhite)
            {
                analogWrite(c1Pin, dwColor * c1 * brightness / 65025);
                analogWrite(c2Pin, dwColor * c2 * brightness / 65025);
            }
            else
            {
                analogWrite(c1Pin, c1 * brightness / 255);
                analogWrite(c2Pin, c2 * brightness / 255);
                analogWrite(c3Pin, c3 * brightness / 255);
            }
        }
        else
        {
            analogWrite(c1Pin, 0);
            analogWrite(c2Pin, 0);
            analogWrite(c3Pin, 0);
        }
    }

    void setOn(bool p)
    {
        on = p;
        setLB();
    }

    void toggleOn()
    {
        setOn(!on);
    }

    void setCo(uint8_t co)
    {
        dwColor = co;
        setLB();
    }

    void incCo(uint8_t co)
    {
        dwColor = (dwColor + co > 255) ? 255 : dwColor + co;
        setLB();
    }

    void decCo(uint8_t co)
    {
        dwColor = (dwColor - co < 0) ? 0 : dwColor - co;
        setLB();
    }

    void setBr(uint8_t br)
    {
        brightness = (brightness < MIN_BR) ? MIN_BR : br;
        setLB();
    }

    void incBr(uint8_t br)
    {
        brightness = (brightness + br > 255) ? 255 : brightness + br;
        setLB();
    }

    void decBr(uint8_t br)
    {
        brightness = (brightness - br < 0) ? 0 : brightness - br;
        if (brightness < MIN_BR)
        {
            brightness = MIN_BR;
        }
        setLB();
    }

    void lc(uint8_t b1)
    {
        c1 = b1;
    }

    void lc(uint8_t b1, uint8_t b2)
    {
        c1 = b1;
        c2 = b2;
    }

    void lc(uint8_t b1, uint8_t b2, uint8_t b3)
    {
        c1 = b1;
        c2 = b2;
        c3 = b3;
    }

    uint8_t getC1()
    {
        return c1;
    }

    uint8_t getC2()
    {
        return c2;
    }

    uint8_t getC3()
    {
        return c3;
    }

    uint8_t getBr()
    {
        return brightness;
    }

    uint8_t getCo()
    {
        return dwColor;
    }

    bool getDwMode(){
        return dualWhite;
    }

    bool getOn(){
        return on;
    }
};