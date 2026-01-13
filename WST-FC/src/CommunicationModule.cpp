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
        communicationInterface = new CommunicationWiFiUDPModule(sharedData, UDP_CONTROLL_PORT, droneStatus);
    }
    if(COMMUNICATION_METHOD == 1){
        communicationInterface = new CommunicationESPNowModule(sharedData, droneStatus);
    }
    if(COMMUNICATION_METHOD == 2){
        communicationInterface = new CommunicationSerialModule(sharedData, droneStatus);
    }

    if(communicationInterface == nullptr)
    {
        //Critical error!!!
    }else{
        communicationInterface->Init();
    }
}

void CommunicationModule::Loop()
{
    if(communicationInterface == nullptr) return;
    communicationInterface -> Loop();
}