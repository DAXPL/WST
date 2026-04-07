#ifndef ESCACTUATOR_H
#define ESCACTUATOR_H

#include "IActuator.h"
#include <ESP32Servo.h>

class ESCActuator : public IActuator {
private:
    Servo _esc;
    int _pin;

    int _minPulse = 0;
    int _maxPulse = 0;
    int _armedPulse = 1000;

public:
    ESCActuator(int pin, int minPulse=1000, int maxPulse=2000)
    {
        _pin = pin;
        _minPulse = minPulse;
        _maxPulse = maxPulse;
    }

    void Init() override {
        _esc.attach(_pin, _minPulse, _maxPulse);
        _esc.writeMicroseconds(_armedPulse);
    }   


    void Set(int16_t speed) override {
        if (speed < 0) speed = 0;
        if (speed > 1000) speed = 1000;

        if (speed > 0 && speed < 200)
            speed = 200;

        int pulse = map(speed, 0, 1000, _minPulse, _maxPulse);
        _esc.writeMicroseconds(pulse);
    }


    void Loop() override {}
};

#endif
