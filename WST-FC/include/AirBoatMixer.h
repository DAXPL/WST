#ifndef AIRBOATMIXER_H
#define AIRBOATMIXER_H

#include "IMixer.h"
#include "IActuator.h"
#include <Arduino.h>
#include "DCMotor.h"
class BoatMixer : public IMixer
{
private:
    IActuator *_motorLeft;
    IActuator *_motorRight;

public:
    BoatMixer()
    {
        _motorRight = new DCMotor(18, 19, 5, 1);
        _motorLeft = new DCMotor(16, 17, 4, 0);
    }

    void Init() override
    {
        if(_motorLeft)_motorLeft->Init();
        if(_motorRight)_motorRight->Init();
    }

    void Update(DroneControlData *input) override
    {
        //"Differential Drive"
        int16_t throttle = input->throttle;
        int16_t yaw = input->yaw;

        int16_t leftSpeed = throttle + yaw;
        int16_t rightSpeed = throttle - yaw;

        if(_motorRight)
        {
            _motorRight->Set(constrain(rightSpeed, -1000, 1000));
            _motorRight->Loop();
        }

        if(_motorLeft)
        {
            _motorLeft->Set(constrain(leftSpeed, -1000, 1000));
            _motorLeft->Loop();
        }
    }

    void StopAll() override
    {
        _motorLeft->Set(0);
        _motorRight->Set(0);
    }
};

#endif