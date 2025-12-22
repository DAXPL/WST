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
    double _output;
    double _kp = 0.5;  // Moc reakcji (Spring strength)
    double _ki = 0.0;  // Pamięć błędu
    double _kd = 0.8;  // Tłumienie
    PID *_pitchPID;

public:
    BicopterMixer()
    {
        _servoLeft = new ServoMotor(16,0,180); 
        _servoRight = new ServoMotor(17,0,180);

        _pitchSetpoint = 0;
        _pitchInput = 0;
        _output = 0;

        _pitchPID = new PID(&_pitchInput, &_output, &_pitchSetpoint, _kp, _ki, _kd, DIRECT);
    }

    void Init() override
    {
        //if(_motorLeft) _motorLeft->Init();
        //if(_motorRight) _motorRight->Init();
        if(_servoLeft) _servoLeft->Init();
        if(_servoRight) _servoRight->Init();
        
        if(_pitchPID)
        {
            _pitchPID->SetMode(AUTOMATIC); // Włączenie PID
            _pitchPID->SetSampleTime(10); // Częstotliwość odświeżania (ms)
            _pitchPID->SetOutputLimits(-9000, 9000);
        }
        
    }

    void Update(DroneControlData *input, SensorsData *sensors) override 
    {
        if(sensors == nullptr || input == nullptr) return;
        _pitchInput = sensors->pitch;
        //_pitchSetpoint = input->pitch;
        _pitchSetpoint = 0;

        if (_pitchPID->Compute()) 
        {
            Serial.printf("%.2f\n", _output / 100.0f);

            if(_servoLeft)
            {
                _servoLeft->Set(-_output);
            } 

            if(_servoRight)
            {
                _servoRight->Set(_output);
            }
        }
        
    }

    void StopAll() override
    {
        if(_servoLeft) _servoLeft->Set(0);
        if(_servoRight) _servoRight->Set(0);
    }
};

#endif