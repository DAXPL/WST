#ifndef BATTERYVOLTAGEDIVIDERSENSOR_H
#define BATTERYVOLTAGEDIVIDERSENSOR_H

#include "../ISensor.h" 
#define MAX_ANALOG_VALUE 4095.0
#define MAX_VOLTAGE 25
#define BATTERY_READ_DELAY 1000
class BatteryVoltageDividerSensor : public ISensor
{
private:
    int inputPin {0};
    int timestamp {0};
public:
    BatteryVoltageDividerSensor(int pin);
    void Init() override;
    void Update(SensorsData *data) override;
};

#endif