#ifndef IMODULE
#define IMODULE

#include "../SensorsData.h"
#include "../CommunicationModule.h"
class IModule{
public:
    virtual ~IModule(){}
    virtual void Init();
    virtual void Loop(CommunicationModule* interface, SensorsData *data);
};
#endif 