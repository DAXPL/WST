#ifndef ADXLSENSOR_H
#define ADXLSENSOR_H

#include "../ISensor.h" 
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

class AdxlSensor : public ISensor
{
private:
    Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
    
    float pitch {0};
    float roll {0};
    bool sensorInitiated {false};
    const float alpha {0.2f}; 

public:
    AdxlSensor();
    void Init() override;
    void Update(SensorsData *data) override;
};

#endif