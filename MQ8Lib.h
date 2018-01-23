/* MQ8 gas sensor library
   Home: https://github.com/kennywakeland/MQLib
   (Based on: https://github.com/xerlay11/MQ-2-sensor-library)
*/ 
#ifndef MQ8Lib_h__
#define MQ8Lib_h__

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

//gases id (also for value indexing)
#define MQ8_LPG    0
#define MQ8_CO     1
#define MQ8_SMOKE  2

//other defines
#define  MQ8_RL_VALUE  5     //define the load resistance on the board, in kilo Ohms
#define  MQ8_RO_CLEAN_AIR_FACTOR  9.83
#define  MQ8_CALIBARAION_SAMPLE_TIMES  5
#define  MQ8_CALIBRATION_SAMPLE_INTERVAL  50
#define  MQ8_READ_SAMPLE_TIMES  5
#define  MQ8_READ_SAMPLE_INTERVAL  50
#define  MQ8_READ_RATE  5000  //define value reading rate [ms] (applies for sensor reading and also for each gas value calculation)

class MQ8
{
public:
	MQ8(short pin, bool doSerial = false);
	float* read(bool print = false);
	float readLPG();
	float readCO();
	float readSmoke();
	void begin();

private:
	unsigned short _pin;
	bool _serial;

	const float LPGCurve[3]  =  {2.3,0.21,-0.47};
	const float COCurve[3]  =  {2.3,0.72,-0.34};
	const float SmokeCurve[3] = {2.3,0.53,-0.44};
	float Ro = 10;  //calculated by calibration

  float read_val = 0; //value from sensor (precalculated)
	float values[3] = {0,0,0}; //calculated gas values {MQ8_LPG,MQ8_CO,MQ8_SMOKE}

	float MQRead();
	float MQGetGasPercentage(float rs_ro_ratio, short gas_id);
	float MQGetPercentage(float rs_ro_ratio, const float *pcurve);
	float MQCalibration();
	float MQResistanceCalculation(int raw_adc);

	unsigned long lastReadTime[4] = {0,0,0,0};  //for reading sensor and calculating each value {sensor,MQ8_LPG,MQ8_CO,MQ8_SMOKE}
};

#endif //MQ8Lib_h__
