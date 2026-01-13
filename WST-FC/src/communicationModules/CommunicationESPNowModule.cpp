#include "communicationModules\CommunicationESPNowModule.h"
#include "Configuration.h"
#include "communicationModules\CommunicationESPNowModule.h"




CommunicationESPNowModule::CommunicationESPNowModule(DroneControlData *dataPtr, DroneStatus *status){
    sharedData = dataPtr;
    droneStatus = status;
    instance = this;
}


void CommunicationESPNowModule::Init()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    if(esp_now_init() != ESP_OK){
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_recv_cb(OnDataReceived);

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if(esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
    
    Serial.print("MAC address: ");
    Serial.println(WiFi.macAddress());
}
void CommunicationESPNowModule::Loop()
{
    if(droneStatus==nullptr) return;
    if(millis() - lastDataTime > MAX_ROGUE_TIME)
    {
        *droneStatus = WARNING;
    }
    else
    {
        *droneStatus = WORKS;
    }
}
void CommunicationESPNowModule::OnDataReceived(const uint8_t *mac, const uint8_t *incomingData, int len) {
    if(len != sizeof(DroneControlData)){
        Serial.println("Received data of incorrect size");
        return;
    }

    DroneControlData receivedData;
    if (instance->sharedData != nullptr) {
        memcpy(instance->sharedData, incomingData, sizeof(DroneControlData));
    }
    instance->lastDataTime = millis();
}