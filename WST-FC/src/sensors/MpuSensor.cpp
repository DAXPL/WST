#include "sensors/MpuSensor.h"

void MpuSensor::Init() {
    if (!mpu.begin()) 
    {
        Serial.println("MPU6050 not found!");
        return;
    }
    
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    mpuInitiated = true;

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    pitch = atan2(a.acceleration.y, a.acceleration.z) * RAD_TO_DEG;
    roll  = atan2(-a.acceleration.x, a.acceleration.z) * RAD_TO_DEG;

    lastTime = millis();
}

void MpuSensor::Update(SensorsData* data) 
{
    if(mpuInitiated == false) return;
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    unsigned long currentTime = millis();
    float dt = (currentTime - lastTime) / 1000.0;
    lastTime = currentTime;

    // Filters
    float accelPitch = atan2(a.acceleration.y, a.acceleration.z) * RAD_TO_DEG;
    float accelRoll = atan2(-a.acceleration.x, a.acceleration.z) * RAD_TO_DEG;

    pitch = 0.96 * (pitch + g.gyro.x * RAD_TO_DEG * dt) + 0.04 * accelPitch;
    roll  = 0.96 * (roll  + g.gyro.y * RAD_TO_DEG * dt) + 0.04 * accelRoll; 

    //Accel
    float radPitch = pitch * DEG_TO_RAD;
    float radRoll  = roll  * DEG_TO_RAD;

    float gravityX = sin(radRoll) * 9.81;
    float gravityY = sin(radPitch) * 9.81;
    float gravityZ = cos(radRoll) * cos(radPitch) * 9.81;

    float linearAccelX = a.acceleration.x - gravityX;
    float linearAccelY = a.acceleration.y - gravityY;
    float linearAccelZ = a.acceleration.z - gravityZ;

    //Output
    data->pitch = (int16_t)(pitch * 100.0f);
    data->roll = (int16_t)(roll * 100.0f);
    data->linearAccelX = (int16_t)(linearAccelX * 100.0f);
    data->linearAccelY= (int16_t)(linearAccelY * 100.0f);
    data->linearAccelZ = (int16_t)(linearAccelZ * 100.0f);
}