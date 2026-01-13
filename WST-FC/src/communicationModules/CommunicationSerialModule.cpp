#include "communicationModules\CommunicationSerialModule.h"
#include "Configuration.h"

CommunicationSerialModule::CommunicationSerialModule(DroneControlData *dataPtr, DroneStatus *status)
{
    sharedData = dataPtr;
    droneStatus = status;
}
void CommunicationSerialModule::Init()
{

}
void CommunicationSerialModule::Loop()
{
    if (sharedData == nullptr) return;
    if (Serial.available() < sizeof(DroneControlData) + 2) return;
    if (Serial.read() != HEADER_BYTE_1) return;
    if (Serial.read() != HEADER_BYTE_2) return;

    Serial.readBytes(inputBuffer, sizeof(DroneControlData));
    memcpy(sharedData, inputBuffer, sizeof(DroneControlData));
    lastUpdate = millis();
    
    

    if ((millis() - lastUpdate) > MAX_ROGUE_TIME) 
    {
        *droneStatus = WARNING;
    } 
    else 
    {
        *droneStatus = WORKS;
    }
}