#ifndef BICOPTERMIXER_H
#define BICOPTERMIXER_H

#include "IMixer.h"
#include "IActuator.h"
#include <Arduino.h>
#include <PID_v1.h>
#include "DCMotor.h"
#include "ISensor.h"
#include "ESCActuator.h"
#include "ServoMotor.h"

class BicopterMixer : public IMixer
{
private:
    IActuator *_motorLeft;
    IActuator *_motorRight;
    IActuator *_servoLeft;
    IActuator *_servoRight;

    // PITCH
    double _pitchSetpoint;
    double _pitchInput;
    double _pitchOutput;
    double _kpPitch = 1.5;
    double _kiPitch = 0.0;
    double _kdPitch = 0.1;

    // ROLL
    double _rollSetpoint;
    double _rollInput;
    double _rollOutput;
    double _kpRoll = 1.0; 
    double _kiRoll = 0.0;
    double _kdRoll = 0.1;
    
    double _pitchFilterState = 0.0;
    const double _pitchFilterAlpha = 0.7;

    PID *_pitchPID;
    PID *_rollPID;

public:
    BicopterMixer()
    {
        _servoLeft = new ServoMotor(16); 
        _servoRight = new ServoMotor(17);

        _motorLeft = new ESCActuator(5);
        _motorRight = new ESCActuator(18); 

        _pitchSetpoint = 0;
        _pitchInput = 0;
        _pitchOutput = 0;

        _rollSetpoint = 0;
        _rollInput = 0;
        _rollOutput = 0;

        _pitchPID = new PID(&_pitchInput, &_pitchOutput, &_pitchSetpoint, _kpPitch, _kiPitch, _kdPitch, DIRECT);
        _rollPID = new PID(&_rollInput, &_rollOutput, &_rollSetpoint, _kpRoll, _kiRoll, _kdRoll, DIRECT);
    }

    void Init() override
    {
        if(_servoLeft)  _servoLeft->Init();
        if(_servoRight) _servoRight->Init();
        if(_motorLeft)  _motorLeft->Init();
        if(_motorRight) _motorRight->Init();
        
        if(_pitchPID)
        {
            _pitchPID->SetMode(AUTOMATIC);
            _pitchPID->SetSampleTime(20); 
            _pitchPID->SetOutputLimits(-9000, 9000);
        }

        if(_rollPID)
        {
            _rollPID->SetMode(AUTOMATIC);
            _rollPID->SetSampleTime(20); 
            _rollPID->SetOutputLimits(-300, 300); 
        }
    }

    void Update(DroneControlData *input, SensorsData *sensors) override 
    {
        if(sensors == nullptr || input == nullptr) return;

        // PITCH
        _pitchFilterState = (_pitchFilterState * (1.0 - _pitchFilterAlpha)) + ((double)(sensors->pitch) * _pitchFilterAlpha);
        _pitchInput = _pitchFilterState;
        _pitchSetpoint = input->pitch;

        if (_pitchPID->Compute()) 
        {
            int16_t controlSignal = _pitchOutput;
            if(_servoLeft) _servoLeft->Set(controlSignal);
            if(_servoRight) _servoRight->Set(-controlSignal); 
        }

        // ROLL
        _rollInput = (double)(sensors->roll);
        _rollSetpoint = (double)input->roll; 
        
        if(_rollPID->Compute())
        {
            //int16_t baseThrottle = input->throttle;
            int16_t baseThrottle = 500;
            int16_t correction = _rollOutput;

            int16_t motorLeftSpeed = baseThrottle + correction;
            int16_t motorRightSpeed = baseThrottle - correction;
            if(_motorLeft) _motorLeft->Set(motorLeftSpeed);
            if(_motorRight) _motorRight->Set(motorRightSpeed);
        }
    }

    void StopAll() override
    {
        if(_servoLeft) _servoLeft->Set(0);
        if(_servoRight) _servoRight->Set(0);
        if(_motorLeft) _motorLeft->Set(0);
        if(_motorRight) _motorRight->Set(0);
    }
};

#endif