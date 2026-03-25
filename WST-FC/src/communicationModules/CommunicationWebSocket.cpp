#include "Configuration.h"
#include "communicationModules/CommunicationWebSocket.h"
CommunicationWebSocket* CommunicationWebSocket::instance = nullptr;
CommunicationWebSocket::CommunicationWebSocket(DroneControlData *dataPtr, DroneStatus *status)
    : server(80), ws("/ws")
{
    sharedData = dataPtr;
    droneStatus = status;
    instance = this;
}
void CommunicationWebSocket::Init(){
    ws.onEvent(onWsEvent);
    server.addHandler(&ws);
    server.begin();
    Serial.println("WebSocket server started on port 80");
}
void CommunicationWebSocket::Loop(){
    checkConnectionTimeout();
    ws.cleanupClients(); 
}
void CommunicationWebSocket::SendData(SensorsData* data){
    ws.textAll((char*)data, sizeof(SensorsData));
}
void CommunicationWebSocket::checkConnectionTimeout() {
    if (millis() - lastDataTime > 500) {
        if (sharedData != nullptr) {
            
            sharedData->throttle = 0; 
            sharedData->yaw = 0;
            sharedData->pitch = 0;
            sharedData->roll = 0;
        }
    }
}
void CommunicationWebSocket::onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, 
                                       AwsEventType type, void *arg, uint8_t *data, size_t len) 
{
    if (type == WS_EVT_CONNECT) {
        Serial.printf("[WS] Client connected! ID: %u\n", client->id());
    } 
    else if (type == WS_EVT_DISCONNECT) {
        Serial.printf("[WS] Client disconnected! ID: %u\n", client->id());
        
        if (instance && instance->sharedData) {
            instance->sharedData->throttle = 0; 
            instance->sharedData->yaw = 0;
            instance->sharedData->pitch = 0;
            instance->sharedData->roll = 0;
        }
    } 
    else if (type == WS_EVT_DATA) {
        AwsFrameInfo *info = (AwsFrameInfo*)arg;
        
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_BINARY) {
            
            if (len == sizeof(DroneControlData) && instance && instance->sharedData) {
                
                memcpy(instance->sharedData, data, sizeof(DroneControlData));
                
                instance->lastDataTime = millis();
            }
        }
    }
}