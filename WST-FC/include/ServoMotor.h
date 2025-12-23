#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

#include "IActuator.h"
#include <Arduino.h>
#include <ESP32Servo.h>

class ServoMotor : public IActuator
{
private:
    Servo _servo;
    int _pin;
    int _minPulse;
    int _maxPulse;
    int _centerAngle;
    int16_t _maxDeflectionScaled; 

public:
    ServoMotor(int pin, int minPulse = 500, int maxPulse = 2400)
    {
        _pin = pin;
        _minPulse = minPulse;
        _maxPulse = maxPulse;
        _centerAngle = 90; 
        _maxDeflectionScaled = 9000; 
    }

    void Init() override
    {
        _servo.setPeriodHertz(50);
        _servo.attach(_pin, _minPulse, _maxPulse);
        Serial.println(_pin);
        Set(0);
    }

    void Set(int16_t value) override
    {
        int targetAngle = _centerAngle + (value / 100);
        targetAngle = constrain(targetAngle, 0, 180);
        _servo.write(targetAngle);
    }

    void Loop() override {}
};

#endif