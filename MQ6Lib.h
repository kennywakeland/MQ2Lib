/* MQ6 gas sensor library
   Home: https://github.com/kennywakeland/MQLib
   (Based on: https://github.com/xerlay11/MQ-2-sensor-library)
   http://sandboxelectronics.com/?p=191
*/
#ifndef MQ6Lib_h__
#define MQ6Lib_h__

#if ARDUINO >= 100

#include "Arduino.h"

#else
#include "WProgram.h"
#endif

#include "MQBase.h"

//gases id (also for value indexing)
#define MQ6_LPG    0
#define MQ6_CH4    1


//other defines
#define  MQ6_RL_VALUE  20     /// define the load resistance on the board, in kilo ohms
#define  MQ6_RO_CLEAN_AIR_FACTOR  10 /// RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO, which is derived from the chart in datasheet
#define  MQ6_CALIBARAION_SAMPLE_TIMES  10 /// define how many samples you are going to take in the calibration phase
#define  MQ6_CALIBRATION_SAMPLE_INTERVAL  100 /// define the time interal(in milisecond) between each samples in the cablibration phase
#define  MQ6_READ_SAMPLE_TIMES  5 /// define the time interal(in milisecond) between each samples in normal operation
#define  MQ6_READ_SAMPLE_INTERVAL  50 /// define how many samples you are going to take in normal operation
#define  MQ6_READ_RATE  5000  /// define value reading rate [ms] (applies for sensor reading and also for each gas value calculation)

class MQ6: public MQBase
{
public:
    MQ6(unsigned short pin, bool doSerial = false);
    float* read(bool print);
    float readRaw();

    float readLPG();
    float readCH4();

    void begin();

private:
    unsigned short _pin;

#ifdef MQ_DEBUG
    bool _serial;
#endif

    /*****************************Globals***********************************************/
    /// two points are taken from the curve.
    /// with these two points, a line is formed which is "approximately equivalent" to the original curve.
    /// data format:{ x, y, slope}; point1: (lg1000, lg1), point2: (lg10000, lg0.4)
    const float LPGCurve[3] = {3, 0, -0.4};


    /// two points are taken from the curve.
    /// with these two points, a line is formed which is "approximately equivalent" to the original curve.
    /// data format:{ x, y, slope}; point1: (lg2000, lg1), point2: (lg5000,  lg0.7)
    const float CH4Curve[3] = {3.3, 0, -0.38};

    ///Ro is initialized to 10 kilo ohms
    float Ro = 10;

    float read_val = 0; //value from sensor (precalculated)
    float values[2] = {0, 0}; //calculated gas values {MQ6_LPG,MQ6_CH4}

    float MQRead();
    float MQGetGasPercentage(float rs_ro_ratio, short gas_id);
    float MQGetPercentage(float rs_ro_ratio, const float* pcurve);
    float MQCalibration();
    float MQResistanceCalculation(int raw_adc);

    ///for reading sensor and calculating each value {sensor,MQ6_LPG,MQ6_CH4}
    unsigned long lastReadTime[3] = {0, 0, 0};

};

#endif //MQ6Lib_h__
