#ifndef COMMUNICATIONMODULE_H
#define COMMUNICATIONMODULE_H

#include <WiFi.h>
#include <WiFiUdp.h>
#include "DroneData.h"
#include "Configuration.h"

class CommunicationModule {
private:
    DroneControlData* sharedData;
    WiFiUDP udp;
    unsigned int localPort{0};
    char packetBuffer[255];

    unsigned long lastUpdate {0};
    int8_t rssi {0};
    wl_status_t connectionStatus {WL_IDLE_STATUS};
    DroneStatus* droneStatus;
public:
    CommunicationModule(DroneControlData* dataPtr, unsigned int port, DroneStatus* status);

    void Init();
    void Loop();
};

#endif