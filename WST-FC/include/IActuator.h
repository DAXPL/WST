#ifndef IMOTOR_H
#define IMOTOR_H
#include <Arduino.h>
class IActuator
{
public:
    virtual ~IActuator() {}
    virtual void Init() = 0;
    virtual void Set(int16_t speed) = 0;
    virtual void Loop() = 0;
};
#endif