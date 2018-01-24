/* MQ8 gas sensor library
   Home: https://github.com/kennywakeland/MQLib
   (Based on: https://github.com/xerlay11/MQ-2-sensor-library)
*/
#include "MQ8Lib.h"

/* ----- constructor
   \param in   pin - sensor pin no.
   \param in   doSerial - true=do serial output [default=false]
*/
MQ8::MQ8(unsigned short pin, bool doSerial)
{
    _pin = pin;

#ifdef MQ_DEBUG
    _serial = doSerial;
#endif
}

/* ----- Init */
void MQ8::begin()
{
    Ro = MQCalibration();

#ifdef MQ_DEBUG
    if(_serial)
    {
        Serial.print(F("MQ8 Ro: "));
        Serial.print(Ro);
        Serial.println(F(" kohm"));
    }
#endif
}

/* ----- Read all
   \param in   print - true=print to serial (if enabled) [default=false]
*/
float* MQ8::read(bool print)
{
    MQRead();
    MQGetGasPercentage(read_val / Ro, MQ8_H2);

#ifdef MQ_DEBUG
    if(print && _serial) //printing to serial (a bit complicated construction, but saving space for string constants)
    {
        for(int i = 0; i < 3; i++)
        {
            if(i == 0)
            {
                Serial.print(F("H2: "));
                Serial.print(values[MQ8_H2]);
            }
            else
                Serial.print(F("  "));


            Serial.print(F("ppm"));
        }
        Serial.print(F("\n"));
    }
#endif

    return values;
}

float MQ8::readRaw()
{
    return MQRead();
}

/* ----- Read H2 */
float MQ8::readH2()
{
    return MQGetGasPercentage(MQRead() / Ro, MQ8_H2);
}


/* ----- Resistance calculation
   \param in   raw_adc - sensor value
*/
float MQ8::MQResistanceCalculation(int raw_adc)
{
    return (((float) MQ8_RL_VALUE * (1023 - raw_adc) / raw_adc));
}

/* ----- Calibration */
float MQ8::MQCalibration()
{
    read_val = 0;
    for(int i = 0; i < MQ8_CALIBARAION_SAMPLE_TIMES; i++) //take multiple samples
    {
        read_val += MQResistanceCalculation(analogRead(_pin));
        delay(MQ8_CALIBRATION_SAMPLE_INTERVAL);
    }

    read_val = read_val / MQ8_CALIBARAION_SAMPLE_TIMES;    //calculate the average value
    read_val = read_val / MQ8_RO_CLEAN_AIR_FACTOR;         //divided by RO_CLEAN_AIR_FACTOR yields the Ro
    //according to the chart in the datasheet
    return read_val;
}

/* ----- Reading */
float MQ8::MQRead()
{
    if(millis() < (lastReadTime[0] + MQ8_READ_RATE))
        return read_val;

    read_val = 0;
    for(int i = 0; i < MQ8_READ_SAMPLE_TIMES; i++)
    {
        read_val += MQResistanceCalculation(analogRead(_pin));
        delay(MQ8_READ_SAMPLE_INTERVAL);
    }

    read_val = read_val / MQ8_READ_SAMPLE_TIMES;    //calculate the average value
    lastReadTime[0] = millis();
    return read_val;
}

/* ----- Calculate specific gas percentage
   \param in   rs_ro_rato - read resistance ratio
   \param in   gas_id - id of gas, see defines
*/
float MQ8::MQGetGasPercentage(float rs_ro_ratio, short gas_id)
{
    if(gas_id != MQ8_H2)
        return 0;

    if(millis() < (lastReadTime[1 + gas_id] + MQ8_READ_RATE))
        return values[gas_id];

    if(gas_id == MQ8_H2)
        values[MQ8_H2] = MQGetPercentage(rs_ro_ratio, H2Curve);
    else
        return 0; //redundant

    lastReadTime[1 + gas_id] = millis();
    return values[gas_id];
}

/* ----- Calculate gas percentage
   \param in   rs_ro_rato - read resistance ratio
   \param in   pcurve - gas curve, see header
*/
float MQ8::MQGetPercentage(float rs_ro_ratio, const float* pcurve)
{
    return (pow(10, (((log(rs_ro_ratio) - pcurve[1]) / pcurve[2]) + pcurve[0])));
}
