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
    int _minAngle;
    int _maxAngle;
    int _minPulse;
    int _maxPulse;

public:
    ServoMotor(int pin, int minAngle = 0, int maxAngle = 180, int minPulse = 500, int maxPulse = 2400)
    {
        _pin = pin;
        _minAngle = minAngle;
        _maxAngle = maxAngle;
        _minPulse = minPulse;
        _maxPulse = maxPulse;
    }

    void Init() override
    {
        _servo.setPeriodHertz(50);
        _servo.attach(_pin, _minPulse, _maxPulse);
        Set(0); 
    }

    void Set(int16_t speed) override
    {
        speed = constrain(speed, -9000, 9000);
        int angle = map(speed, -9000, 9000, _minAngle, _maxAngle);
        Serial.printf("Servo: %d deg\n",angle);
        _servo.write(angle);
    }

    void Loop() override {}
};

#endif