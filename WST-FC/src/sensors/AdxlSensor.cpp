#include "sensors/AdxlSensor.h"

AdxlSensor::AdxlSensor(){

}
void AdxlSensor::Init() {
    if (!accel.begin()) 
    {
        Serial.println("ADXL345 not found!");
        return;
    }

    accel.setRange(ADXL345_RANGE_8_G);
    accel.setDataRate(ADXL345_DATARATE_100_HZ);

    sensorInitiated = true;
    sensors_event_t event;
    accel.getEvent(&event);
    pitch = atan2(event.acceleration.y, event.acceleration.z) * RAD_TO_DEG;
    roll  = atan2(-event.acceleration.x, event.acceleration.z) * RAD_TO_DEG;
}

void AdxlSensor::Update(SensorsData* data) 
{
    if(!sensorInitiated) return;

    sensors_event_t event;
    accel.getEvent(&event);

    float currentPitch = atan2(event.acceleration.y, event.acceleration.z) * RAD_TO_DEG;
    float currentRoll  = atan2(-event.acceleration.x, event.acceleration.z) * RAD_TO_DEG;

    // Low Pass Filter
    pitch = (pitch * (1.0f - alpha)) + (currentPitch * alpha);
    roll  = (roll  * (1.0f - alpha)) + (currentRoll  * alpha);

    float radPitch = pitch * DEG_TO_RAD;
    float radRoll  = roll  * DEG_TO_RAD;

    float gravityX = sin(radRoll) * 9.81;
    float gravityY = sin(radPitch) * 9.81;
    float gravityZ = cos(radRoll) * cos(radPitch) * 9.81;

    float linearAccelX = event.acceleration.x - gravityX;
    float linearAccelY = event.acceleration.y - gravityY;
    float linearAccelZ = event.acceleration.z - gravityZ;

    data->pitch = (int16_t)(pitch * 100.0f);
    data->roll  = (int16_t)(roll * 100.0f);
    
    data->linearAccelX = (int16_t)(linearAccelX * 100.0f);
    data->linearAccelY = (int16_t)(linearAccelY * 100.0f);
    data->linearAccelZ = (int16_t)(linearAccelZ * 100.0f);
}