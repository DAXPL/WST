#ifndef ISENSOR_H
#define ISENSOR_H

#include "SensorsData.h"

class ISensor {
public:
    virtual ~ISensor() {}

    virtual void Init() = 0;
    virtual void Update(SensorsData* data) = 0;
};

#endif