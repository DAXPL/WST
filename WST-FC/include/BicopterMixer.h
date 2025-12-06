#ifndef BICOPTERMIXER_H
#define BICOPTERMIXER_H

#include "IMixer.h"
#include "IActuator.h"
#include <Arduino.h>
#include "DCMotor.h"
class BicopterMixer : public IMixer
{
private:
    IActuator *_motorLeft;
    IActuator *_motorRight;
    IActuator *_servoLeft;
    IActuator *_servoRight;

public:
    BicopterMixer()
    {
        _motorLeft = new DCMotor(16, 17, 4, 0);
        _motorRight = new DCMotor(18, 19, 5, 1);
        //_servoLeft = ;
        //_servoRight = ;
    }

    void Init() override
    {
        if(_motorLeft)_motorLeft->Init();
        if(_motorRight)_motorRight->Init();
        if(_servoLeft)_servoLeft->Init();
        if(_servoRight)_servoRight->Init();
    }

    void Update(DroneControlData *input) override {}

    void StopAll() override
    {
        if(_motorLeft)_motorLeft->Set(0);
        if(_motorRight)_motorRight->Set(0);
        if(_servoLeft)_servoLeft->Set(0);
        if(_servoRight)_servoRight->Set(0);
    }
};

#endif