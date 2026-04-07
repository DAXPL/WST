#ifndef DCMOTOR_H
#define DCMOTOR_H

#include "IActuator.h"

class DCMotor : public IActuator
{
private:
    int _pinIN1, _pinIN2, _pinEN;
    int _pwmChannel;
    bool _revDir;
    bool _hasENPin;

public:
    DCMotor(int pinIN1, int pinIN2, int pinEN, int pwmChannel, bool revDir = false)
    {
        _pinIN1 = pinIN1;
        _pinIN2 = pinIN2;
        _pinEN = pinEN;
        _pwmChannel = pwmChannel;
        _revDir = revDir;
        _hasENPin = true;
    }

    DCMotor(int pinIN1, int pinIN2, int pwmChannel, bool revDir = false)
    {
        _pinIN1 = pinIN1;
        _pinIN2 = pinIN2;
        _pinEN = -1;
        _pwmChannel = pwmChannel;
        _revDir = revDir;
        _hasENPin = false;
    }

    void Init() override
    {
        ledcSetup(_pwmChannel, 1000, 8); // 1kHz, 8 bit (0-255)
        pinMode(_pinIN1, OUTPUT);
        pinMode(_pinIN2, OUTPUT);
        if(_hasENPin)
        {
            ledcAttachPin(_pinEN, _pwmChannel);
        }
        else
        {
            digitalWrite(_pinIN1, LOW);
            digitalWrite(_pinIN2, LOW);
        }
    }

    void Set(int16_t speed) override
    {
        int pwmValue = map(abs(speed), 0, 1000, 0, 255);
        if (pwmValue > 255) pwmValue = 255;
        
        if(_revDir == true) speed = -speed;

        if (_hasENPin)
        {
            // 3PIN
            if (speed > 0)
            {
                digitalWrite(_pinIN1, HIGH);
                digitalWrite(_pinIN2, LOW);
            }
            else if (speed < 0)
            {
                digitalWrite(_pinIN1, LOW);
                digitalWrite(_pinIN2, HIGH);
            }
            else
            {
                digitalWrite(_pinIN1, LOW);
                digitalWrite(_pinIN2, LOW);
            }
            ledcWrite(_pwmChannel, pwmValue);
        }
        else
        {
            // 2 PIN
            if (speed > 0)
            {
                ledcDetachPin(_pinIN2); 
                pinMode(_pinIN2, OUTPUT);
                digitalWrite(_pinIN2, LOW);
                
                ledcAttachPin(_pinIN1, _pwmChannel);
                ledcWrite(_pwmChannel, pwmValue);
            }
            else if (speed < 0)
            {
                ledcDetachPin(_pinIN1);
                pinMode(_pinIN1, OUTPUT);
                digitalWrite(_pinIN1, LOW);

                ledcAttachPin(_pinIN2, _pwmChannel);
                ledcWrite(_pwmChannel, pwmValue);
            }
            else
            {
                ledcDetachPin(_pinIN1);
                ledcDetachPin(_pinIN2);
                pinMode(_pinIN1, OUTPUT);
                pinMode(_pinIN2, OUTPUT);
                digitalWrite(_pinIN1, LOW);
                digitalWrite(_pinIN2, LOW);
            }
        }
    }

    void Loop() override {}
};

#endif