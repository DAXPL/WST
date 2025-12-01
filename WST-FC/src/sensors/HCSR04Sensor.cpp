#include "sensors/HCSR04Sensor.h"

/*--- INTERRUPT ---*/
void IRAM_ATTR HCSR04Sensor::ISR(void* arg) 
{
    HCSR04Sensor* self = static_cast<HCSR04Sensor*>(arg);
    self->HandleInterrupt();
}

void HCSR04Sensor::HandleInterrupt() 
{
    if (digitalRead(echoPin) == HIGH) 
    {
        startTime = micros();
    }
    else 
    {
        if (startTime > 0) 
        {
            echoDuration = micros() - startTime;
            newDataAvailable = true;
            startTime = 0;
        }
    }
}

/*--- CLASSS ---*/
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