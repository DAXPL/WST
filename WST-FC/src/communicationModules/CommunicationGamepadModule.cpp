#include "communicationModules/CommunicationGamepadModule.h"
CommunicationGamepadModule::CommunicationGamepadModule(DroneControlData *dataPtr, DroneStatus *status){
    sharedData = dataPtr;
    droneStatus = status;
}
void CommunicationGamepadModule::Init(){
    #if defined(USE_PAD_XBOX)
        xbox.begin();
    #elif defined(USE_PAD_PS4)
        PS4.begin(DRONE_MAC_ADDRESS);
    #elif defined(USE_PAD_PS5)
        ps5.begin(DRONE_MAC_ADDRESS);
    #endif
}

void CommunicationGamepadModule::Loop(){
    bool isConnected = false;

    #if defined(USE_PAD_XBOX)
        isConnected = xbox.isConnected();
    #elif defined(USE_PAD_PS4)
        isConnected = PS4.isConnected();
    #elif defined(USE_PAD_PS5)
        isConnected = ps5.isConnected();
    #endif

    if (!isConnected) {
        return; 
    }

    #if defined(USE_PAD_XBOX)
        XboxControlsState s;
        xbox.read(&s);
        sharedData->throttle = (int16_t)(s.leftStickY * 32767.0f);
        sharedData->yaw      = (int16_t)(s.leftStickX * 32767.0f);
        sharedData->pitch    = (int16_t)(s.rightStickY * 32767.0f);
        sharedData->roll     = (int16_t)(s.rightStickX * 32767.0f);

    #elif defined(USE_PAD_PS4)
        sharedData->throttle = PS4.data.analog.stick.ly;
        sharedData->yaw      = PS4.data.analog.stick.lx;
        sharedData->pitch    = PS4.data.analog.stick.ry;
        sharedData->roll     = PS4.data.analog.stick.rx;

    #elif defined(USE_PAD_PS5)
        sharedData->throttle = ps5.LStickY();
        sharedData->yaw      = ps5.LStickX();
        sharedData->pitch    = ps5.RStickY();
        sharedData->roll     = ps5.RStickX();
    #endif

}
void CommunicationGamepadModule::SendData(SensorsData* data){

}
