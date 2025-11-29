#include "CommunicationModule.h"
#include "Configuration.h"
/*
DNS-0.0.0.0
packet loss
*/
CommunicationModule::CommunicationModule(DroneControlData* dataPtr, unsigned int port, DroneStatus* status) {
    sharedData = dataPtr;
    localPort = port;
    droneStatus = status;
}

void CommunicationModule::Init() 
{
    Serial.print("Connecting to WiFi");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) 
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nConnected!\nIP Address: "+WiFi.localIP());
    udp.begin(localPort);
}

void CommunicationModule::Loop() 
{
    rssi = WiFi.RSSI();
    connectionStatus = WiFi.status();

    int packetSize = udp.parsePacket();

    if (packetSize) 
    {
        int len = udp.read(packetBuffer, 255);

        if (len != sizeof(DroneControlData)) 
        {
            Serial.printf("Error: invalid packet");
            return;
        }
        memcpy(sharedData, packetBuffer, sizeof(DroneControlData));
        lastUpdate = millis();
    }

    if(rssi < -80 || connectionStatus!= WL_CONNECTED || (millis()-lastUpdate)>MAX_ROGUE_TIME)
    {
        *droneStatus = WARNING;
    }
    else
    {
        *droneStatus = WORKS;
    }
}