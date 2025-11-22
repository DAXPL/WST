#ifndef SENSORSMODULE_H
#define SENSORSMODULE_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "DroneData.h"
#include "SensorsData.h"

class SensorsModule {
private:
    SensorsData* sensorData;
    Adafruit_MPU6050 mpu;
    float _pitch = 0;
    float _roll = 0;
    
    unsigned long _lastTime = 0;

    bool mpuInitiated {false};
public:
    SensorsModule(SensorsData* data); 
    void Init();
    void Loop();
};

#endif