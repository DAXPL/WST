#ifndef COMMUNICATIONWIFIUDPMODULE_H
#define COMMUNICATIONWIFIUDPMODULE_H

#include <WiFi.h>
#include <WiFiUdp.h>
#include "DroneData.h"
#include "Configuration.h"
class CommunicationWiFiUDPModule{
    private:
    DroneControlData *sharedData;
    WiFiUDP udp;
    unsigned int localPort{0};
    char packetBuffer[255];

    unsigned long lastUpdate{0};
    int8_t rssi{0};
    wl_status_t connectionStatus{WL_IDLE_STATUS};
    DroneStatus *droneStatus;

    public:
    CommunicationWiFiUDPModule(DroneControlData *dataPtr, unsigned int port, DroneStatus *status);

    void Init();
    void Loop();
};
#endif