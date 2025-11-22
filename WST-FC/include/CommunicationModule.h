#ifndef COMMUNICATIONMODULE_H
#define COMMUNICATIONMODULE_H

#include <WiFi.h>
#include <WiFiUdp.h>
#include "DroneData.h"

class CommunicationModule {
private:
    DroneControlData* sharedData;
    WiFiUDP udp;
    unsigned int localPort{0};
    char packetBuffer[255];
    unsigned long lastUpdate {0};
public:
    CommunicationModule(DroneControlData* dataPtr, unsigned int port);

    void Init();
    void Loop();
};

#endif