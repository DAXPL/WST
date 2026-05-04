#ifndef ICOMMUNICATIONINTERFACE
#define ICOMMUNICATIONINTERFACE

#include "../SensorsData.h"
#include "SystemRegistry.h"
#include "Configuration.h"
class ICommunicationInterface{
protected:
    DroneControlData *sharedData;
    DroneStatus *droneStatus;
    unsigned long lastUpdate{0};

    void ParseIncomingBytes(uint8_t* buffer, size_t len) 
    {
        if (sharedData == nullptr || len < 2) return;

        if (len == sizeof(DroneControlData)) 
        {
            memcpy(sharedData, buffer, sizeof(DroneControlData));
            lastUpdate = millis();
        }
        else if (buffer[0] == 'S' && buffer[1] == 'E' && buffer[2] == 'T') 
        {
            if (len < 64) buffer[len] = '\0'; 
            else buffer[63] = '\0';
        
            char* str = (char*)buffer; 
            char* cmd = strtok(str, " ");
            char* paramName = strtok(NULL, " ");
            char* valueStr = strtok(NULL, " ");

            if (paramName != nullptr && valueStr != nullptr) 
            {
                SystemRegistry::GetInstance().SetValue(paramName, valueStr);
            }
        }
        else if (buffer[0] == 'G' && buffer[1] == 'E' && buffer[2] == 'T') 
        {
            if (len < 64) buffer[len] = '\0'; 
            else buffer[63] = '\0';
            SystemRegistry::GetInstance().ReturnAllRegistryNames();
        }
    }

public:
    virtual ~ICommunicationInterface(){}
    virtual void Init();
    virtual void Loop();
    virtual void SendData(SensorsData* data);
};
#endif 