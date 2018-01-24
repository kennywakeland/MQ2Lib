/* MQ4 gas sensor library
 * The Curve is wrong
   Home: https://github.com/kennywakeland/MQLib
   (Based on: https://github.com/xerlay11/MQ-2-sensor-library)
   http://sandboxelectronics.com/?p=196
*/
#ifndef MQ4Lib_h__
#define MQ4Lib_h__

#if ARDUINO >= 100

#include "Arduino.h"

#else

#include "WProgram.h"

#endif

#include "MQBase.h"

//gases id (also for value indexing)
#define MQ4_H2    0

//other defines
#define  MQ4_RL_VALUE  10     /// define the load resistance on the board, in kilo Ohms
#define  MQ4_RO_CLEAN_AIR_FACTOR  9.21  /// RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO, which is derived from the chart in datasheet
#define  MQ4_CALIBARAION_SAMPLE_TIMES  10 /// define how many samples you are going to take in the calibration phase
#define  MQ4_CALIBRATION_SAMPLE_INTERVAL  100 /// define the time interal(in milisecond) between each samples in the cablibration phase
#define  MQ4_READ_SAMPLE_TIMES  5 /// define the time interal(in milisecond) between each samples in normal operation
#define  MQ4_READ_SAMPLE_INTERVAL  50 /// define how many samples you are going to take in normal operation
#define  MQ4_READ_RATE  5000  //define value reading rate [ms] (applies for sensor reading and also for each gas value calculation)

class MQ4: public MQBase
{
public:
    MQ4(unsigned short pin, bool doSerial = false);
    float* read(bool print);
    float readRaw();

    float readH2();

    void begin();

private:
    unsigned short _pin;

#ifdef MQ_DEBUG
    bool _serial;
#endif

    /// two points are taken from the curve in datasheet.
    /// with these two points, a line is formed which is "approximately equivalent" to the original curve.
    /// data format:{ x, y, slope}; point1: (lg200, lg8.5), point2: (lg10000, lg0.03)
    /// ToDo: Fix Curve
    const float H2Curve[3] = {2.3, 0.93, -1.44};

    ///calculated by calibration
    float Ro = 10;

    float read_val = 0; //value from sensor (precalculated)
    float values[1] = {0}; //calculated gas values {MQ4_H2}

    float MQRead();
    float MQGetGasPercentage(float rs_ro_ratio, short gas_id);
    float MQGetPercentage(float rs_ro_ratio, const float* pcurve);
    float MQCalibration();
    float MQResistanceCalculation(int raw_adc);

    ///for reading sensor and calculating each value {sensor,MQ4_H2}
    unsigned long lastReadTime[4] = {0, 0};
};

#endif //MQ4Lib_h__
