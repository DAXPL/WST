#include "sensors/HCSR04Sensor.h"

void IRAM_ATTR HCSR04Sensor::ISR(void* arg) 
{
    HCSR04Sensor* self = static_cast<HCSR04Sensor*>(arg);
    self->HandleInterrupt();
}

void IRAM_ATTR HCSR04Sensor::HandleInterrupt() 
{
    if (digitalRead(echoPin) == HIGH) 
    {
        portENTER_CRITICAL_ISR(&mux);
        startTime = micros();
        portEXIT_CRITICAL_ISR(&mux);
    }
    else 
    {
        unsigned long endTime = micros();
        portENTER_CRITICAL_ISR(&mux);
        if (startTime > 0) 
        {
            echoDuration = endTime - startTime;
            newDataAvailable = true;
            startTime = 0;
        }
        portEXIT_CRITICAL_ISR(&mux);
    }
}

HCSR04Sensor::HCSR04Sensor(int trig, int echo, int id) : trigPin(trig), echoPin(echo), id(id) 
{
}

void HCSR04Sensor::Init() 
{
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    digitalWrite(trigPin, LOW);

    attachInterruptArg(digitalPinToInterrupt(echoPin), ISR, this, CHANGE);
}

void HCSR04Sensor::Update(SensorsData* data) 
{
    unsigned long now = millis();

    // Timeout 
    // Maksymalny zasięg czujnika to ok 4m, co daje ok. 24ms czasu echa.
    if (_state == WAITING_FOR_ECHO && (now - _lastPingTime > 30)) 
    {
        portENTER_CRITICAL_ISR(&mux);
        newDataAvailable = false; 
        startTime = 0;
        portEXIT_CRITICAL_ISR(&mux);
        _state = IDLE;
    }

    // Odbiór danych
    if (newDataAvailable) 
    {
        portENTER_CRITICAL_ISR(&mux); 
        unsigned long duration = echoDuration;
        newDataAvailable = false;
        portEXIT_CRITICAL_ISR(&mux);

        long distance = duration * 0.034 / 2;
        
        if (id >= 0 && id < 6) 
        {
            data->distanceSensors[id] = (uint16_t)distance;
        }
        
        _state = IDLE;
    }

    // Wyzwalanie nowego pomiaru
    if (_state == IDLE && (now - _lastPingTime > PING_INTERVAL)) 
    {
        _lastPingTime = now;
        _state = WAITING_FOR_ECHO;
        
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
    }
}