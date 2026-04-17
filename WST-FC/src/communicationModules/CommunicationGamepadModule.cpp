#include "communicationModules/CommunicationGamepadModule.h"
#ifdef USE_BLUEPAD32
CommunicationGamepadModule* CommunicationGamepadModule::instance = nullptr;

void CommunicationGamepadModule::onConnectedGamepad(GamepadPtr gp) {
    if (instance != nullptr && instance->GetGamepad() == nullptr) {
        Serial.printf("Gamepad connected: %s\n", gp->getModelName().c_str());
        instance->SetGamepad(gp);
    }
}

void CommunicationGamepadModule::onDisconnectedGamepad(GamepadPtr gp) {
    if (instance != nullptr) {
        Serial.println("Gamepad disconnected!");
        instance->SetGamepad(nullptr);
    }
}

CommunicationGamepadModule::CommunicationGamepadModule(DroneControlData *dataPtr, DroneStatus *status){
    sharedData = dataPtr;
    droneStatus = status;
    instance = this;
}
void CommunicationGamepadModule::Init(){
    Serial.println("Initializing Bluepad32...");
    BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);
}

void CommunicationGamepadModule::Loop(){
    BP32.update();

    if (myGamepad && myGamepad->isConnected()) {

        const int inRange = 511;
        const int outRange= 1000;
       
        sharedData->throttle =  map(myGamepad->axisY(),  -inRange, inRange, -outRange, outRange);
        sharedData->yaw      =  map(myGamepad->axisX(),  -inRange, inRange, -outRange, outRange);
        sharedData->pitch    =  map(myGamepad->axisRY(), -inRange, inRange, -outRange, outRange); 
        sharedData->roll     =  map(myGamepad->axisRX(), -inRange, inRange, -outRange, outRange);
    }
}

void CommunicationGamepadModule::SendData(SensorsData* data){

}
void CommunicationGamepadModule::SetGamepad(GamepadPtr gp) { 
    myGamepad = gp;
}
#endif