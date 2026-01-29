#ifndef COMMUNICATIONSERIALMODULE_H
#define COMMUNICATIONSERIALMODULE_H

#include <Arduino.h>
#include "DroneData.h"
#include "Configuration.h"
#include "ICommunicationInterface.h"

class CommunicationSerialModule : public ICommunicationInterface{
    private:
        DroneControlData *sharedData;
        unsigned long lastUpdate{0};
        DroneStatus *droneStatus;
        long lastDataTime = 0;
        uint8_t inputBuffer[sizeof(DroneControlData)];

        const uint8_t HEADER_BYTE_1 = 0x44; // 'D'
        const uint8_t HEADER_BYTE_2 = 0x43; // 'C'
    public:
    CommunicationSerialModule(DroneControlData *dataPtr, DroneStatus *status);
    void Init() override;
    void Loop() override;
    void SendData(SensorsData* data) override;
};
#endif