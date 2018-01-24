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

float lpg = 0, co = 0, smoke = 0;

///instance (true=with serial output enabled)
MQ2 mq2(mq2_pin, true);
MQ6 mq6(mq6_pin, true);
MQ8 mq8(mq8_pin, true);

MQ4 mq4(mq4_pin, true);
MQ9 mq9(mq9_pin, true);

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
    Serial.print(F("-----------\n"));

    /* Read all values from the sensor - it returns an array which contains 3 values
    * 1 = LPG in ppm
    * 2 = CO in ppm
    * 3 = SMOKE in ppm
    */
    float* values = mq2.read(true); //set it false if you don't want to print the values in the Serial


    //Reading specific values:
    //lpg = values[0];
    lpg = mq2.readLPG();
    //co = values[1];
    co = mq2.readCO();
    //smoke = values[2];
    smoke = mq2.readSmoke();

    values = mq6.read(true);
    values = mq8.read(true);

    values = mq4.read(true);
    values = mq9.read(true);

    Serial.print(F("\n"));

    //do something...

    delay(2000); //but values are read according to the rate defined in the library
}
