#ifndef IMIXER_H
#define IMIXER_H

#include "DroneData.h"

class IMixer
{
public:
    virtual ~IMixer() {}
    virtual void Init() = 0;
    virtual void Update(DroneControlData *input, SensorsData *sensors) = 0;
    virtual void StopAll() = 0;
};

#endif