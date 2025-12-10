#ifndef AIRBOATMIXER_H
#define AIRBOATMIXER_H

#include "IMixer.h"
#include "IActuator.h"
#include <Arduino.h>
#include "DCMotor.h"
#include "ISensor.h"

#define MAX_DC_SPEED 1000
class BoatMixer : public IMixer
{
private:
    IActuator *_motorLeft;
    IActuator *_motorRight;

public:
    BoatMixer(IActuator* left, IActuator* right) 
        : _motorLeft(left), _motorRight(right) {}

    void Init() override
    {
        if(_motorLeft)_motorLeft->Init();
        if(_motorRight)_motorRight->Init();
    }

    void Update(DroneControlData *input, SensorsData *sensors) override
    {
        //"Differential Drive"
        int16_t throttle = input->throttle;
        int16_t yaw = input->yaw;

        int16_t leftSpeed = constrain(throttle + yaw, -MAX_DC_SPEED, MAX_DC_SPEED);
        int16_t rightSpeed = constrain(throttle - yaw, -MAX_DC_SPEED, MAX_DC_SPEED);

        if(_motorRight)
        {
            _motorRight->Set(rightSpeed);
            _motorRight->Loop();
        }

        if(_motorLeft)
        {
            _motorLeft->Set(leftSpeed);
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