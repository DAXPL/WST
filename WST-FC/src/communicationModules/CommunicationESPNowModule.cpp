#include "communicationModules\CommunicationESPNowModule.h"
#include "Configuration.h"

CommunicationESPNowModule::CommunicationESPNowModule(DroneControlData *dataPtr, DroneStatus *status){
    sharedData = dataPtr;
    droneStatus = status;
}

void CommunicationESPNowModule::Init(){

    WiFi.mode(WIFI_STA);

    Serial.print("MAC address: ");
    Serial.println(WiFi.macAddress());
    

}
void CommunicationESPNowModule::Loop(){

}