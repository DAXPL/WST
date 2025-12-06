#ifndef COMMUNICATIONMODULE_H
#define COMMUNICATIONMODULE_H

#include <WiFi.h>
#include <WiFiUdp.h>
#include "DroneData.h"
#include "Configuration.h"
#include "communicationModules\CommunicationWiFiUDPModule.h"

class CommunicationModule
{
private:
    DroneControlData *sharedData;
    CommunicationWiFiUDPModule *wifimodule;
    wl_status_t connectionStatus{WL_IDLE_STATUS};
    DroneStatus *droneStatus;

public:
    CommunicationModule(DroneControlData *dataPtr, DroneStatus *status);

    void Init();
    void Loop();
};

#endif