#include "sensors/DHT11Sensor.h"

DHT11Sensor::DHT11Sensor(int pin, int id) : _dht(pin, DHTTYPE), _id(id)
{
}

void DHT11Sensor::Init() 
{
    _dht.begin();
}

void DHT11Sensor::Update(SensorsData* data) 
{
    unsigned long now = millis();
    if (now - _lastReadTime > READ_INTERVAL) 
    {
        _lastReadTime = now;

        float temp = _dht.readTemperature();
        float hum = _dht.readHumidity();

        if (isnan(temp) || isnan(hum)) 
        {
            return;
        }
        
        if (_id >= 0 && _id < 4)
        {
            data->other[_id]   = (int16_t)(temp * 100.0f);
            data->other[_id+1] = (int16_t)(hum * 100.0f);
        }
    }
}