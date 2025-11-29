#ifndef MPUSENSOR_H
#define MPUSENSOR_H
#include "../ISensor.h"
#include <Adafruit_MPU6050.h>
#include <Wire.h>

class MpuSensor : public ISensor {
private:
    Adafruit_MPU6050 mpu;
    float pitch = 0;
    float roll = 0;
    unsigned long lastTime = 0;
    bool mpuInitiated = false;

public:
    void Init() override;
    void Update(SensorsData* data) override;
};

#endif