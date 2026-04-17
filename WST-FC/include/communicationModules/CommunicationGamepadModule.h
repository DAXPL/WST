#ifndef COMMUNICATIONGAMEPADMODULE_H
#define COMMUNICATIONGAMEPADMODULE_H
#ifdef USE_BLUEPAD32
#include "DroneData.h"
#include "ICommunicationInterface.h"
#include "Configuration.h"
#include <Bluepad32.h>


class CommunicationGamepadModule : public ICommunicationInterface {
    private:
        DroneControlData *sharedData;
        DroneStatus *droneStatus;
        GamepadPtr myGamepad = nullptr;

    public:
        static CommunicationGamepadModule* instance;

        CommunicationGamepadModule(DroneControlData *dataPtr, DroneStatus *status);
        
        void Init() override;
        void Loop() override;
        void SendData(SensorsData* data) override;
        void SetGamepad(GamepadPtr gp);
        GamepadPtr GetGamepad() { return myGamepad; }

        static void onConnectedGamepad(GamepadPtr gp);
        static void onDisconnectedGamepad(GamepadPtr gp);
};
#endif
#endif