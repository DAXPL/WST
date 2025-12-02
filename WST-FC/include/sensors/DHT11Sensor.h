#ifndef DHT11SENSOR_H
#define DHT11SENSOR_H
#include "../ISensor.h"
#include <DHT.h>
#include <Wire.h>
#define DHTTYPE DHT11
class DHT11Sensor : public ISensor
{
private:
    DHT _dht;
    int _id {0};

    unsigned long _lastReadTime {0};
    const unsigned long READ_INTERVAL {2000};
public:
    DHT11Sensor(int pin, int id);
    void Init() override;
    void Update(SensorsData *data) override;
};

#endif