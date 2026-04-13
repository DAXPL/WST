#ifndef DRONEFACTORY_H
#define DRONEFACTORY_H

#include <Arduino.h>
#include <vector>
#include "SensorsModule.h"
#include "modules/IModule.h"
#include "IMixer.h"

class DroneFactory
{
public:
    static IMixer* BuildVehicle(SensorsModule& sensorsModule, std::vector<IModule*>& modules);
};

#endif