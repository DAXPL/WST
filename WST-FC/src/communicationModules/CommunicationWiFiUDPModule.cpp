#include "communicationModules\CommunicationWiFiUDPModule.h"
#include "Configuration.h"


CommunicationWiFiUDPModule::CommunicationWiFiUDPModule(DroneControlData *dataPtr, unsigned int port, DroneStatus *status)
{
    sharedData = dataPtr;
    localPort = port;
    droneStatus = status;
}

void CommunicationWiFiUDPModule::Init()
{
    Serial.print("Connecting to WiFi");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(500);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.print("Connected!");

    #ifdef USE_WIREGUARD
        configTime(9 * 60 * 60, 0, "ntp.jst.mfeed.ad.jp", "ntp.nict.jp", "time.google.com");
        time_t now = time(nullptr);
        while (now < 8 * 3600 * 2) 
        {
            delay(500);
            Serial.print(".");
            now = time(nullptr);
        }
        Serial.println("\nTime synchronized!");

        wg.begin(
            IPAddress().fromString(WG_LOCAL_IP),
            WG_PRIVATE_KEY,
            WG_PEER_ADDRESS,
            WG_PEER_PUBLIC_KEY,
            WG_PEER_PORT
        );
        Serial.println("WireGuard initialized.");
    #endif

    udp.begin(localPort);
}
void CommunicationWiFiUDPModule::Loop()
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
        remoteIP = udp.remoteIP();
        remotePort = udp.remotePort();
    }
    if (rssi < MIN_RSSI || connectionStatus != WL_CONNECTED || (millis() - lastUpdate) > MAX_ROGUE_TIME)
    {
        *droneStatus = WARNING;
    }
    else
    {
        *droneStatus = WORKS;
    }
}

void CommunicationWiFiUDPModule::SendData(SensorsData* data)
{
    if(remotePort==0 || data == nullptr) return;
    udp.beginPacket(remoteIP, remotePort);
    udp.write((const uint8_t*)data, sizeof(SensorsData));
    udp.endPacket();
}