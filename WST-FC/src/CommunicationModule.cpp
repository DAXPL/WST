#include "CommunicationModule.h"
#include "Configuration.h"
/*
Problemy:
-co jak za długo nie ma odpowiedzi? -> landingmachen()
-co jak utracimy połączenie z wifi? -> landingmachen()
*/
CommunicationModule::CommunicationModule(DroneControlData* dataPtr, unsigned int port) {
    sharedData = dataPtr;
    localPort = port;
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
    }
}