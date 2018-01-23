//
// Created by kenny on 1/23/18.
//

#include <Arduino.h>

#include "MQ2Lib.h"
#include "MQ6Lib.h"

int mq2_pin = A0; //change this to the pin that you use
int mq6_pin = A0; //change this to the pin that you use
float lpg = 0, co = 0, smoke = 0;

MQ2 mq2(mq2_pin, true); //instance (true=with serial output enabled)
MQ6 mq6(mq6_pin, true); //instance (true=with serial output enabled)

void setup()
{
    Serial.begin(9600);
    mq2.begin();
    mq6.begin();
}

void loop()
{
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
    //do something...

    delay(2000); //but values are read according to the rate defined in the library
}
