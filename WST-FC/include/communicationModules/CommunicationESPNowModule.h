#ifndef COMMUNICATIONESPNOWMODULE_H
#define COMMUNICATIONESPNOWMODULE_H

#include <WiFi.h>
#include "DroneData.h"
#include "Configuration.h"
#include "ICommunicationInterface.h"
#include <esp_now.h>


class CommunicationESPNowModule : public ICommunicationInterface{
    private:
    DroneControlData *sharedData;
    unsigned long lastUpdate{0};
    DroneStatus *droneStatus;

    public:
    CommunicationESPNowModule(DroneControlData *dataPtr, DroneStatus *status);
    
    void Init();
    void Loop();
};

#endif