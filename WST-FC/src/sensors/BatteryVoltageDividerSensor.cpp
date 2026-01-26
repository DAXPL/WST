#include "sensors/BatteryVoltageDividerSensor.h"
BatteryVoltageDividerSensor::BatteryVoltageDividerSensor(int pin)
{
    inputPin = pin;
}

void BatteryVoltageDividerSensor::Init(){}

void BatteryVoltageDividerSensor::Update(SensorsData* data)
{
    if(millis()-timestamp<BATTERY_READ_DELAY) return;
    data->voltage = (analogRead(inputPin)/MAX_ANALOG_VALUE)*MAX_VOLTAGE;
    timestamp = millis();
}