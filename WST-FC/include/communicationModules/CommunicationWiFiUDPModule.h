#ifndef COMMUNICATIONWIFIUDPMODULE_H
#define COMMUNICATIONWIFIUDPMODULE_H

#include <WiFi.h>
#include <WiFiUdp.h>
#include "DroneData.h"
#include "ICommunicationInterface.h"
#include "Configuration.h"
#ifdef USE_WIREGUARD
    #include <WireGuard-ESP32.h>
#endif
class CommunicationWiFiUDPModule : public ICommunicationInterface{
    private:
    WiFiUDP udp;
    unsigned int localPort{0};
    char packetBuffer[255];

    int8_t rssi{0};
    wl_status_t connectionStatus{WL_IDLE_STATUS};

    IPAddress remoteIP;
    unsigned int remotePort {0};

    #ifdef USE_WIREGUARD
        WireGuard wg;
        unsigned long lastKeepaliveTime = 0;
        const unsigned long KEEPALIVE_INTERVAL = 20000;
        const uint8_t KEEPALIVE_BYTE = 0xFF;
    #endif

    public:
    CommunicationWiFiUDPModule(DroneControlData *dataPtr, unsigned int port, DroneStatus *status);

    void Init() override;
    void Loop() override;
    void SendData(SensorsData* data) override;
};
#endif