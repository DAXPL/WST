#ifndef HCSRSENSOR_H
#define HCSRSENSOR_H
#include "../ISensor.h"
#include <Wire.h>

class HCSR04Sensor : public ISensor
{
private:
    int trigPin {0};
    int echoPin {0};
    int id {0};

    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    volatile unsigned long startTime {0};
    volatile unsigned long echoDuration {0};
    volatile bool newDataAvailable {false};

    enum State {IDLE, WAITING_FOR_ECHO};
    State _state {IDLE};

    unsigned long _lastPingTime {0};
    const unsigned long PING_INTERVAL {60};

    static void IRAM_ATTR ISR(void* arg);
    void HandleInterrupt();

public:
    HCSR04Sensor(int trig, int echo, int id);
    void Init() override;
    void Update(SensorsData *data) override;
};

#endif