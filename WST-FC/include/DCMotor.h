#ifndef DCMOTOR_H
#define DCMOTOR_H

#include "IActuator.h"

class DCMotor : public IActuator
{
private:
    int _pinIN1, _pinIN2, _pinEN;
    int _pwmChannel;

public:
    DCMotor(int pinIN1, int pinIN2, int pinEN, int pwmChannel)
    {
        _pinIN1 = pinIN1;
        _pinIN2 = pinIN2;
        _pinEN = pinEN;
        _pwmChannel = pwmChannel;
    }

    void Init() override
    {
        pinMode(_pinIN1, OUTPUT);
        pinMode(_pinIN2, OUTPUT);

        ledcSetup(_pwmChannel, 1000, 8); // 1kHz, 8 bit (0-255)
        ledcAttachPin(_pinEN, _pwmChannel);
    }

    void Set(int16_t speed) override
    {
        int pwmValue = map(abs(speed), 0, 1000, 0, 255);
        if (pwmValue > 255)
            pwmValue = 255;

        if (speed > 0)
        {
            // Forward
            digitalWrite(_pinIN1, HIGH);
            digitalWrite(_pinIN2, LOW);
        }
        else if (speed < 0)
        {
            // Backward
            digitalWrite(_pinIN1, LOW);
            digitalWrite(_pinIN2, HIGH);
        }
        else
        {
            // Stop
            digitalWrite(_pinIN1, LOW);
            digitalWrite(_pinIN2, LOW);
        }

        ledcWrite(_pwmChannel, pwmValue);
    }

    void Loop() override {}
};

#endif