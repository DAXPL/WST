#include "SensorsModule.h"

SensorsModule::SensorsModule(SensorsData *data)
{
    sensorData = data;
}

void SensorsModule::AddSensor(ISensor *sensor)
{
    _sensors.push_back(sensor);
}

void SensorsModule::Init()
{
    Wire.begin();
    for (auto sensor : _sensors)
    {
        sensor->Init();
    }
}

void SensorsModule::Loop()
{
    for (auto sensor : _sensors)
    {
        sensor->Update(sensorData);
    }
}
