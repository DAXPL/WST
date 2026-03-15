#ifndef COMMUNICATIONROUTER_H
#define COMMUNICATIONROUTER_H
#include "communicationModules/ICommunicationInterface.h"
#include "../DroneData.h"

class CommunicationRouter:public ICommunicationInterface{
    public:
    ICommunicationInterface* communicationInterfaceOne;
    ICommunicationInterface* communicationInterfaceTwo;
    void Init()override{
        if(communicationInterfaceOne != nullptr) communicationInterfaceOne->Init();
        if(communicationInterfaceTwo != nullptr) communicationInterfaceTwo->Init();
    }
    void Loop()override{
        if(communicationInterfaceOne != nullptr) communicationInterfaceOne->Loop();
        if(communicationInterfaceTwo != nullptr) communicationInterfaceTwo->Loop();
    }
    void SendData(SensorsData* data)override{
        communicationInterfaceTwo->SendData(data);
    }
    void SendData(DroneControlData* data)override{
        communicationInterfaceOne->SendData(data);
    }
};

#endif