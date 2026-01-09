#include "communicationModules\CommunicationESPNowModule.h"
#include "Configuration.h"

ulong lastDataTime = 0;
const ulong timeout = 500;

CommunicationESPNowModule::CommunicationESPNowModule(DroneControlData *dataPtr, DroneStatus *status){
    sharedData = dataPtr;
    droneStatus = status;
}
uint8_t broadcastAddress[] = {0xEC,0x64,0xC9,0xC4,0xA2,0x1A}; //tu będzie do uzupełnienia MAC odbiorcy

void CommunicationESPNowModule::Init()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    esp_now_init();
    esp_now_register_recv_cb(onDataReceived);

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
    
    Serial.print("MAC address: ");
    Serial.println(WiFi.macAddress());
}
void CommunicationESPNowModule::Loop(){

    //tu zrobimy użycie odebranych danych do sterowania dronem
    if(millis() - lastDataTime > timeout){
        Serial.println("No data - ERROR 404");
    }
}
void onDataReceived(const uint8_t *mac, const uint8_t *incomingData, int len) {
    if(len != sizeof(DroneControlData)){
        Serial.println("Received data of incorrect size");
        return;
    }

    DroneControlData receivedData;
    memcpy(&receivedData, incomingData, sizeof(DroneControlData));
    lastDataTime = millis();
    
    Serial.println("Received:");
    Serial.print("Throttle: ");
    Serial.println(receivedData.throttle);
    Serial.print("Yaw: ");
    Serial.println(receivedData.yaw);
    Serial.print("Pitch: ");
    Serial.println(receivedData.pitch);
    Serial.print("Roll: ");
    Serial.println(receivedData.roll);
}