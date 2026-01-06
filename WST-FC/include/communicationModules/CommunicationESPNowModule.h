#ifndef COMMUNICATIONESPNOWMODULE_H
#define COMMUNICATIONESPNOWMODULE_H

#include <WiFi.h>
#include "DroneData.h"
#include "Configuration.h"
#include <esp_now.h>


class CommunicationESPNowModule{
    private:
    DroneControlData *sharedData;
    unsigned long lastUpdate{0};
    DroneStatus *droneStatus;

    public:
    CommunicationESPNowModule(DroneControlData *dataPtr, DroneStatus *status);

    void setup();
    void Init();
    void Loop();
};

#endif