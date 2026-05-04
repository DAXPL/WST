#include "CommunicationModule.h"
#include "Configuration.h"

CommunicationModule::CommunicationModule(DroneControlData *dataPtr, DroneStatus *status)
{
    sharedData = dataPtr;
    droneStatus = status;
}

void CommunicationModule::Init()
{
    #ifdef USE_WIFI_UDP
        communicationInterfaces.push_back(new CommunicationWiFiUDPModule(sharedData, UDP_CONTROLL_PORT, droneStatus));
    #endif
    
    #ifdef USE_ESP_NOW
        communicationInterfaces.push_back( new CommunicationESPNowModule(sharedData, droneStatus));
    #endif

    #ifdef USE_SERIAL
        communicationInterfaces.push_back(new CommunicationSerialModule(sharedData, droneStatus));
    #endif

    #ifdef USE_GAMEPAD
        communicationInterfaces.push_back(new CommunicationGamepadModule(sharedData, droneStatus));
    #endif

    for(auto interface : communicationInterfaces) 
    {
        interface->Init();
    }

}

void CommunicationModule::Loop()
{
    for(auto interface : communicationInterfaces) 
    {
        interface->Loop();
    };
}
void CommunicationModule::SendData(SensorsData* data)
{
    for(auto interface : communicationInterfaces) 
    {
        interface->SendData(data);
    };
}