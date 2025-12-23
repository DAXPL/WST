#ifndef BICOPTERMIXER_H
#define BICOPTERMIXER_H

#include "IMixer.h"
#include "IActuator.h"
#include <Arduino.h>
#include <PID_v1.h>
#include "DCMotor.h"
#include "ISensor.h"
#include "ServoMotor.h"

class BicopterMixer : public IMixer
{
private:
    IActuator *_motorLeft;
    IActuator *_motorRight;
    IActuator *_servoLeft;
    IActuator *_servoRight;

    double _pitchSetpoint;
    double _pitchInput;
    double _pitchOutput;
    double _kpPitch = 1.5;
    double _kiPitch = 0.0;
    double _kdPitch = 0.1;
    double _pitchFilterState = 0.0;
    const double _pitchFilterAlpha = 0.7;
    PID *_pitchPID;

public:
    BicopterMixer()
    {
        _servoLeft = new ServoMotor(16); 
        _servoRight = new ServoMotor(17);

        _pitchSetpoint = 0;
        _pitchInput = 0;
        _pitchOutput = 0;

        _pitchPID = new PID(&_pitchInput, &_pitchOutput, &_pitchSetpoint, _kpPitch, _kiPitch, _kdPitch, DIRECT);
    }

    void Init() override
    {
        if(_servoLeft)  _servoLeft->Init();
        if(_servoRight) _servoRight->Init();
        
        if(_pitchPID)
        {
            _pitchPID->SetMode(AUTOMATIC);
            _pitchPID->SetSampleTime(20); // PID refresh rate for servos - same as servo - 50hz!
            _pitchPID->SetOutputLimits(-9000, 9000);
        }
    }

    void Update(DroneControlData *input, SensorsData *sensors) override 
    {
        if(sensors == nullptr || input == nullptr) return;
        //EMA / LPF filter
        _pitchFilterState = (_pitchFilterState * (1.0 - _pitchFilterAlpha)) + ((double)(sensors->pitch) * _pitchFilterAlpha);

        _pitchInput = _pitchFilterState;
        _pitchSetpoint = input->pitch;
        
        if (_pitchPID->Compute()) 
        {
            int16_t controlSignal = _pitchOutput;
            Serial.println(controlSignal); 
            if(_servoLeft) _servoLeft->Set(controlSignal);
            if(_servoRight) _servoRight->Set(-controlSignal);
        }
    }

    void StopAll() override
    {
        if(_servoLeft) _servoLeft->Set(0);
        if(_servoRight) _servoRight->Set(0);
    }
};

#endif