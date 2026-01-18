#ifndef COMMUNICATIONMODULE_H
#define COMMUNICATIONMODULE_H

#include <WiFi.h>
#include <WiFiUdp.h>
#include "DroneData.h"
#include "Configuration.h"
#include "communicationModules\ICommunicationInterface.h"
#include "communicationModules\CommunicationWiFiUDPModule.h"
#include "communicationModules\CommunicationESPNowModule.h"
#include "communicationModules\CommunicationSerialModule.h"

class CommunicationModule
{
private:
    DroneControlData *sharedData;
    ICommunicationInterface *communicationInterface;
    wl_status_t connectionStatus{WL_IDLE_STATUS};
    DroneStatus *droneStatus;

public:
    CommunicationModule(DroneControlData *dataPtr, DroneStatus *status);

    void Init();
    void Loop();
    void SendData(SensorsData* data);
};

#endif