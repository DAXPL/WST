#ifndef IMOTOR_H
#define IMOTOR_H
#include <Arduino.h>
class IActuator
{
public:
    virtual ~IActuator() {}
    virtual void Init() = 0;
    //Zakładamy sygnał od -1000 do 1000 dla wszystkich actuatorów
    virtual void Set(int16_t speed) = 0;
    virtual void Loop() = 0;
};
#endif