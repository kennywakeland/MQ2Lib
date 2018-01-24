//
// Created by kenny on 1/24/18.
//

#ifndef MQLIB_MQBASE_H
#define MQLIB_MQBASE_H

//#define MQ_DEBUG

class MQBase
{

public:
    virtual void begin() = 0;
    //float* read();
    virtual float* read(bool print) = 0;
    virtual float readRaw() = 0;

};


#endif //MQLIB_MQBASE_H
