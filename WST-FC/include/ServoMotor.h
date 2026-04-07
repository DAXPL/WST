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
    ServoMotor(int pin, int center = 90, int minPulse = 500, int maxPulse = 2400)
    {
        _pin = pin;
        _minPulse = minPulse;
        _maxPulse = maxPulse;
        _centerAngle = center; 
        _maxDeflectionScaled = 9000; 
    }

    void Init() override
    {
        _servo.setPeriodHertz(50);
        _servo.attach(_pin, _minPulse, _maxPulse);
        Serial.println(_pin);
        Set(1500);
    }

    void Set(int16_t value) override
    {
        int pulse = constrain(value, _minPulse, _maxPulse);
        _servo.writeMicroseconds(pulse);
    }

    void Loop() override {}
};

#endif