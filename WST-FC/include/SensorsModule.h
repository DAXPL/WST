#ifndef SENSORSMODULE_H
#define SENSORSMODULE_H

#include <vector>
#include <Wire.h>
#include "DroneData.h"
#include "SensorsData.h"
#include "ISensor.h"

class SensorsModule
{
private:
    SensorsData *sensorData;
    std::vector<ISensor *> _sensors;

public:
    SensorsModule(SensorsData *data);
    void AddSensor(ISensor *sensor);
    void Init();
    void Loop();
};

#endif