#ifndef ICOMMUNICATIONINTERFACE
#define ICOMMUNICATIONINTERFACE

#include "../SensorsData.h"
class ICommunicationInterface{
public:
    virtual ~ICommunicationInterface(){}
    virtual void Init();
    virtual void Loop();
    virtual void SendData(SensorsData* data);
    virtual void SendData(DroneControlData* data);
};
#endif 