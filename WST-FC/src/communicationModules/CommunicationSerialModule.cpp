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
    int bytesAvailable = Serial.available();
    
    if (bytesAvailable > 0)
    {
        size_t bytesToRead = min(bytesAvailable, (int)sizeof(inputBuffer));
        size_t bytesRead = Serial.readBytes(inputBuffer, bytesToRead);
        ParseIncomingBytes((uint8_t*)inputBuffer, bytesRead);
    }

    if ((millis() - lastUpdate) > MAX_ROGUE_TIME) 
    {
        *droneStatus = WARNING;
    } 
    else 
    {
        *droneStatus = WORKS;
    }
}
void CommunicationSerialModule::SendData(SensorsData* data)
{

}