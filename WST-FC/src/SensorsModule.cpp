#include "SensorsModule.h"

SensorsModule::SensorsModule(SensorsData* data) {
    sensorData = data;
}

void SensorsModule::Init() {
    Wire.begin();
    if (!mpu.begin()) 
    {
        Serial.println("MPU6050 not found!");
    }
    else
    {
        mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
        mpu.setGyroRange(MPU6050_RANGE_500_DEG);
        mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
        mpuInitiated = true;
    }
    
    

    _lastTime = millis();
}

void SensorsModule::Loop() {
    // MPU6050
    if(mpuInitiated)
    {
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);

        unsigned long currentTime = millis();
        float dt = (currentTime - _lastTime) / 1000.0;
        _lastTime = currentTime;

        // Filters
        float accelPitch = atan2(a.acceleration.y, a.acceleration.z) * 180 / PI;
        float accelRoll = atan2(-a.acceleration.x, a.acceleration.z) * 180 / PI;
        float accelYaw = atan2(-a.acceleration.z, a.acceleration.z) * 180 / PI;

        _pitch = 0.96 * (_pitch + g.gyro.x * 57.295 * dt) + 0.04 * accelPitch;
        _roll  = 0.96 * (_roll  + g.gyro.y * 57.295 * dt) + 0.04 * accelRoll;

        // Output
        sensorData->pitch = _pitch;
        sensorData->roll = _roll;
    } 
}