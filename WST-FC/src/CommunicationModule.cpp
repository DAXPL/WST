#include "CommunicationModule.h"
#include "Configuration.h"

CommunicationModule::CommunicationModule(DroneControlData *dataPtr, DroneStatus *status)
{
    sharedData = dataPtr;
    droneStatus = status;
}

void CommunicationModule::Init()
{
    wifimodule = new CommunicationWiFiUDPModule(sharedData, UDP_CONTROLL_PORT, droneStatus);
    wifimodule -> Init();
}

void CommunicationModule::Loop()
{
    wifimodule -> Loop();
}