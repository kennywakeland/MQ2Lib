//
// Created by kenny on 1/23/18.
//

#include <Arduino.h>

#include "MQ2Lib.h"
#include "MQ6Lib.h"
#include "MQ8Lib.h"

#include "MQ4Lib.h"
#include "MQ9Lib.h"

/// change this to the pin that you use
#define mq4_pin  A0
#define mq6_pin  A0
#define mq8_pin  A0

#define mq2_pin  A1
#define mq9_pin  A2


///instance (true=with serial output enabled)
MQ2 mq2(mq2_pin, false);
MQ6 mq6(mq6_pin, false);
MQ8 mq8(mq8_pin, false);

MQ4 mq4(mq4_pin, false);
MQ9 mq9(mq9_pin, false);

char KW_ROW[] = " ROW: ";

char KW_LPG[] = " LPG: ";
char KW_SMOKE[] = " SMOKE: ";
char KW_CO[] = " CO: ";
char KW_CH4[] = " CH4: ";
char KW_H2[] = " H2: ";

float* values = NULL;

void setup()
{
    Serial.begin(9600);

    mq2.begin();
    mq6.begin();
    mq8.begin();

    mq4.begin();
    mq9.begin();
}

void loop()
{
    Serial.print(F("----------\n"));


    {
        values = mq2.read(false);
        Serial.print(F("MQ2#"));

        Serial.print(KW_ROW);
        Serial.print(mq2.readRaw());

        Serial.print(KW_LPG);
        Serial.print(values[MQ2_LPG]);

        Serial.print(KW_CO);
        Serial.print(values[MQ2_CO]);

        Serial.print(KW_SMOKE);
        Serial.print(values[MQ2_SMOKE]);
        Serial.print(F("\n"));
    }

    {
        values = mq4.read(false);
        Serial.print(F("MQ4#"));

        Serial.print(KW_ROW);
        Serial.print(mq4.readRaw());

        Serial.print(F("\n"));
    }

    {
        values = mq6.read(false);
        Serial.print(F("MQ6#"));

        Serial.print(KW_ROW);
        Serial.print(mq6.readRaw());

        Serial.print(KW_LPG);
        Serial.print(values[MQ6_LPG]);

        Serial.print(KW_CH4);
        Serial.print(values[MQ6_CH4]);

        Serial.print(F("\n"));
    }


    {
        values = mq8.read(false);
        Serial.print(F("MQ8#"));

        Serial.print(KW_ROW);
        Serial.print(mq8.readRaw());

        Serial.print(KW_H2);
        Serial.print(values[MQ8_H2]);

        Serial.print(F("\n"));
    }

    {
        values = mq9.read(false);
        Serial.print(F("MQ9#"));

        Serial.print(KW_ROW);
        Serial.print(mq9.readRaw());

        Serial.print(F("\n"));
    }

    Serial.print(F("**********\n"));

    //do something...

    delay(1000); //but values are read according to the rate defined in the library
}
