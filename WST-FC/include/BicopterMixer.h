#ifndef BICOPTERMIXER_H
#define BICOPTERMIXER_H

#include "IMixer.h"
#include "IActuator.h"
#include <Arduino.h>

class BicopterMixer : public IMixer {
private:
    IActuator* _motorLeft;
    IActuator* _motorRight;
    IActuator* _servoLeft;
    IActuator* _servoRight;

public:
    BicopterMixer(IActuator* mL, IActuator* mR, IActuator* sL, IActuator* sR) 
    {
        _motorLeft = mL;
        _motorRight = mR;
        _servoLeft = sL;
        _servoRight = sR;
    }

    void Init() override 
    {
        _motorLeft->Init(); 
        _motorRight->Init();
        _servoLeft->Init(); 
        _servoRight->Init();
    }

    void Update(DroneControlData* input) override {}

    void StopAll() override 
    {
        _motorLeft->Set(0); 
        _motorRight->Set(0);
        _servoLeft->Set(0); 
        _servoRight->Set(0);
    }
};

#endif