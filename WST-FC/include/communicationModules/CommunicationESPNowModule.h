#ifndef COMMUNICATIONESPNOWMODULE_H
#define COMMUNICATIONESPNOWMODULE_H

#include <WiFi.h>
#include "DroneData.h"
#include "Configuration.h"
#include "ICommunicationInterface.h"
#include <esp_now.h>


class CommunicationESPNowModule : public ICommunicationInterface{
    private:
    static CommunicationESPNowModule* instance;
    DroneControlData *sharedData;
    unsigned long lastUpdate{0};
    DroneStatus *droneStatus;
    ulong lastDataTime = 0;
    uint8_t broadcastAddress[6] {0xEC,0x64,0xC9,0xC4,0xA2,0x1A};
    public:
    CommunicationESPNowModule(DroneControlData *dataPtr, DroneStatus *status);
    
    void Init() override;
    void Loop() override;
    void SendData(SensorsData* data) override;
    static void OnDataReceived(const uint8_t *mac, const uint8_t *incomingData, int len);
};

#endif