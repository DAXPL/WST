#ifndef COMMUNICATIONSERIALMODULE_H
#define COMMUNICATIONSERIALMODULE_H

#include <Arduino.h>
#include "DroneData.h"
#include "Configuration.h"
#include "ICommunicationInterface.h"

class CommunicationSerialModule : public ICommunicationInterface{
    private:
        long lastDataTime = 0;
        uint8_t inputBuffer[128];

    public:
    CommunicationSerialModule(DroneControlData *dataPtr, DroneStatus *status);
    void Init() override;
    void Loop() override;
    void SendData(SensorsData* data) override;
};
#endif