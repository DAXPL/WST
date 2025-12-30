#include "communicationModules\CommunicationESPNowModule.h"
#include "Configuration.h"

CommunicationESPNowModule::CommunicationESPNowModule(DroneControlData *dataPtr, DroneStatus *status){
    sharedData = dataPtr;
    droneStatus = status;
}
uint8_t broadcastAddress[] = {0xEC,0x64,0xC9,0xC4,0xA2,0x1A}; //tu będzie do uzupełnienia MAC odbiorcy

void CommunicationESPNowModule::setup(){
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);

    if(esp_now_init() != ESP_OK){
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if(esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
}

void CommunicationESPNowModule::Init(){

    Serial.print("MAC address: ");
    Serial.println(WiFi.macAddress());
    

}
void CommunicationESPNowModule::Loop(){

}