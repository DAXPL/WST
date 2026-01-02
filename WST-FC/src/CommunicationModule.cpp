#include "CommunicationModule.h"
#include "Configuration.h"

CommunicationModule::CommunicationModule(DroneControlData *dataPtr, DroneStatus *status)
{
    sharedData = dataPtr;
    droneStatus = status;
}

void CommunicationModule::Init()
{
    if(COMMUNICATION_METHOD == 0){
        wifimodule = new CommunicationWiFiUDPModule(sharedData, UDP_CONTROLL_PORT, droneStatus);
        wifimodule -> Init();
    }
    if(COMMUNICATION_METHOD == 1){
        //blablabla
    }
}

void CommunicationModule::Loop()
{
    if(wifimodule!=nullptr){
        wifimodule -> Loop();
    }
}