#ifndef AIRBOATMIXER_H
#define AIRBOATMIXER_H

#include "IMixer.h"
#include "IActuator.h"
#include <Arduino.h>

class BoatMixer : public IMixer {
private:
    IActuator* _motorLeft;
    IActuator* _motorRight;

public:
    BoatMixer(IActuator* left, IActuator* right) 
    {
        _motorLeft = left;
        _motorRight = right;
    }

    void Init() override 
    {
        _motorLeft->Init();
        _motorRight->Init();
    }

    void Update(DroneControlData* input) override 
    {
        //"Differential Drive"
        int16_t throttle = input->throttle;
        int16_t yaw = input->yaw;

        int16_t leftSpeed  = throttle + yaw;
        int16_t rightSpeed = throttle - yaw;

        _motorLeft->Set(constrain(leftSpeed, -1000, 1000));
        _motorRight->Set(constrain(rightSpeed, -1000, 1000));
        
        _motorLeft->Loop();
        _motorRight->Loop();
    }

    void StopAll() override 
    {
        _motorLeft->Set(0);
        _motorRight->Set(0);
    }
};

#endif