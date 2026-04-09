#ifndef COMMUNICATIONGAMEPADMODULE_H
#define COMMUNICATIONGAMEPADMODULE_H

#include "DroneData.h"
#include "ICommunicationInterface.h"
#include "Configuration.h"

#if defined(USE_PAD_XBOX)
    #include <BLEGamepadClient.h>
#elif defined(USE_PAD_PS4)
    #include <PS4Controller.h>
    #define DRONE_MAC_ADDRESS "1a:2b:3c:01:01:01" 
#elif defined(USE_PAD_PS5)
    #include <ps5Controller.h>
    #define DRONE_MAC_ADDRESS "1a:2b:3c:01:01:01" 
#endif

class CommunicationGamepadModule : public ICommunicationInterface{
    private:
        DroneControlData *sharedData;
        unsigned long lastUpdate{0};
        DroneStatus *droneStatus;

        #if defined(USE_PAD_XBOX)
            XboxController xbox;
        #endif

    public:
        CommunicationGamepadModule(DroneControlData *dataPtr, DroneStatus *status);
        void Init() override;
        void Loop() override;
        void SendData(SensorsData* data) override;
};
#endif