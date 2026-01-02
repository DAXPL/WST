#ifndef COMMUNICATIONESPNOWMODULE_H
#define COMMUNICATIONESPNOWMODULE_H

#include <WiFi.h>
#include "DroneData.h"
#include "Configuration.h"

class CommunicationESPNowModule{
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