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
    double _kdPitch = 0.0;

    // ROLL
    double _rollSetpoint;
    double _rollInput;
    double _rollOutput;
    double _kpRoll = 1.5; 
    double _kiRoll = 0.0;
    double _kdRoll = 0.1;
    
    double _pitchFilterState = 0.0;
    const double _pitchFilterAlpha = 0.7;

    PID *_pitchPID;
    PID *_rollPID;

public:
    BicopterMixer()
    {
        //pin, center angle
        _servoLeft = new ServoMotor(32,90); 
        _servoRight = new ServoMotor(33,90);

        //pin, minPulse, maxPulse
        _motorLeft = new ESCActuator(25,700,1500);
        _motorRight = new ESCActuator(26,700,1500); 

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
        delay(2000);
        if(_motorLeft) _motorLeft->Set(0);
        if(_motorRight) _motorRight->Set(0);

        if(_pitchPID)
        {
            _pitchPID->SetMode(AUTOMATIC);
            _pitchPID->SetSampleTime(20); 
            _pitchPID->SetOutputLimits(-500, 500);
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
        
        // PITCH - Serwa
        _pitchFilterState = (_pitchFilterState * (1.0 - _pitchFilterAlpha)) + ((double)(sensors->pitch) * _pitchFilterAlpha);
        _pitchInput = _pitchFilterState;
        _pitchSetpoint = input->pitch;

        if (_pitchPID->Compute()) 
        {
            int16_t controlSignal = _pitchOutput;
            
            // Przywracamy środek wychylenia (1500 us)
            int16_t servoLeftVal = 1500 + controlSignal;
            int16_t servoRightVal = 1500 - controlSignal;

            if(_servoLeft) _servoLeft->Set(servoLeftVal);
            if(_servoRight) _servoRight->Set(servoRightVal); 
        }

        // ROLL - Silniki
        _rollInput = (double)(sensors->roll);
        _rollSetpoint = (double)input->roll; 
        
        if(_rollPID->Compute())
        {
            int16_t mappedThrottle = map(input->throttle, -1000, 1000, 0, 1000);

            int16_t correction = _rollOutput;
            int16_t motorLeftSpeed = mappedThrottle + correction;
            int16_t motorRightSpeed = mappedThrottle - correction;
            
            if(_motorLeft) _motorLeft->Set(motorLeftSpeed);
            if(_motorRight) _motorRight->Set(motorRightSpeed);
        }
    }

    void StopAll(){};
};

#endif