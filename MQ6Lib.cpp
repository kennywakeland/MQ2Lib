/* MQ6 gas sensor library
   Home: https://github.com/kennywakeland/MQLib
   (Based on: https://github.com/xerlay11/MQ-2-sensor-library)
*/
#include "MQ6Lib.h"

/* ----- constructor
   \param in   pin - sensor pin no.
   \param in   doSerial - true=do serial output [default=false]
*/
MQ6::MQ6(unsigned short pin, bool doSerial)
{
    _pin = pin;

#ifdef MQ_DEBUG
    _serial = doSerial;
#endif
}

/* ----- Init */
void MQ6::begin()
{
    Ro = MQCalibration();

#ifdef MQ_DEBUG
    if(_serial)
    {
        Serial.print(F("MQ6 Ro: "));
        Serial.print(Ro);
        Serial.println(F(" kohm"));
    }
#endif
}

/* ----- Read all
   \param in   print - true=print to serial (if enabled) [default=false]
*/
float* MQ6::read(bool print)
{
    MQRead();
    MQGetGasPercentage(read_val / Ro, MQ6_LPG);
    MQGetGasPercentage(read_val / Ro, MQ6_CH4);

#ifdef MQ_DEBUG
    if(print && _serial) //printing to serial (a bit complicated construction, but saving space for string constants)
    {
        for(int i = 0; i < 2; i++)
        {
            if(i == 0)
            {
                Serial.print(F("LPG: "));
                Serial.print(values[MQ6_LPG]);
            }
            else
                Serial.print(F("  "));

            if(i == 1)
            {
                Serial.print(F("CH4: "));
                Serial.print(values[MQ6_CH4]);
            }

            Serial.print(F("ppm"));
        }
        Serial.print(F("\n"));
    }
#endif

    return values;
}

float MQ6::readRaw()
{
    return MQRead();
}

/* ----- Read LPG */
float MQ6::readLPG()
{
    return MQGetGasPercentage(MQRead() / Ro, MQ6_LPG);
}

/* ----- Read CH4 */
float MQ6::readCH4()
{
    return MQGetGasPercentage(MQRead() / Ro, MQ6_CH4);
}

/* ----- Resistance calculation
   \param in   raw_adc - sensor value
*/
float MQ6::MQResistanceCalculation(int raw_adc)
{
    return (((float) MQ6_RL_VALUE * (1023 - raw_adc) / raw_adc));
}

/* ----- Calibration */
float MQ6::MQCalibration()
{
    read_val = 0;
    for(int i = 0; i < MQ6_CALIBARAION_SAMPLE_TIMES; i++) //take multiple samples
    {
        read_val += MQResistanceCalculation(analogRead(_pin));
        delay(MQ6_CALIBRATION_SAMPLE_INTERVAL);
    }

    read_val = read_val / MQ6_CALIBARAION_SAMPLE_TIMES;    //calculate the average value
    read_val = read_val / MQ6_RO_CLEAN_AIR_FACTOR;         //divided by RO_CLEAN_AIR_FACTOR yields the Ro
    //according to the chart in the datasheet
    return read_val;
}

/* ----- Reading */
float MQ6::MQRead()
{
    if(millis() < (lastReadTime[0] + MQ6_READ_RATE))
        return read_val;

    read_val = 0;
    for(int i = 0; i < MQ6_READ_SAMPLE_TIMES; i++)
    {
        read_val += MQResistanceCalculation(analogRead(_pin));
        delay(MQ6_READ_SAMPLE_INTERVAL);
    }

    read_val = read_val / MQ6_READ_SAMPLE_TIMES;    //calculate the average value
    lastReadTime[0] = millis();
    return read_val;
}

/* ----- Calculate specific gas percentage
   \param in   rs_ro_rato - read resistance ratio
   \param in   gas_id - id of gas, see defines
*/
float MQ6::MQGetGasPercentage(float rs_ro_ratio, short gas_id)
{
    if(gas_id != MQ6_LPG && gas_id != MQ6_CH4)
        return 0;

    if(millis() < (lastReadTime[1 + gas_id] + MQ6_READ_RATE))
        return values[gas_id];

    if(gas_id == MQ6_LPG)
        values[MQ6_LPG] = MQGetPercentage(rs_ro_ratio, LPGCurve);
    else if(gas_id == MQ6_CH4)
        values[MQ6_CH4] = MQGetPercentage(rs_ro_ratio, CH4Curve);
    else
        return 0; //redundant

    lastReadTime[1 + gas_id] = millis();
    return values[gas_id];
}

/* ----- Calculate gas percentage
   \param in   rs_ro_rato - read resistance ratio
   \param in   pcurve - gas curve, see header
*/
float MQ6::MQGetPercentage(float rs_ro_ratio, const float* pcurve)
{
    return (pow(10, (((log(rs_ro_ratio) - pcurve[1]) / pcurve[2]) + pcurve[0])));
}
