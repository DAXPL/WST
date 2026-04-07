#ifndef COMMUNICATIONWEBSOCKET_H
#define COMMUNICATIONWEBSOCKET_H

#include <WiFi.h>
#include <AsyncTCP.h>         
#include <ESPAsyncWebServer.h>  
#include "DroneData.h"
#include "Configuration.h"
#include "ICommunicationInterface.h"

class CommunicationWebSocket : public ICommunicationInterface {
private:
    static CommunicationWebSocket* instance;
    DroneControlData *sharedData;
    DroneStatus *droneStatus;
    
    unsigned long lastUpdate{0};
    unsigned long lastDataTime = 0; 

    AsyncWebServer server;
    AsyncWebSocket ws;

public:
    CommunicationWebSocket(DroneControlData *dataPtr, DroneStatus *status);
    
    void Init() override;
    void Loop() override;
    void SendData(SensorsData* data) override;
    void SendData(DroneControlData* data);

  
    static void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
                          
    void checkConnectionTimeout(); 
};

#endif