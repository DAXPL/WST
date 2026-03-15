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

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    WiFi.setHostname("WST_DRONE");
    // IPAddress ip(10,0,0,99);
    // IPAddress gateway(10,0,0,1);
    // IPAddress subnet(255,255,255,0);
    // WiFi.config(ip, gateway, subnet);
    //to do ustalić adresacje maliny wewn. dla dronów po wifi
    //to do ustawić routingi pomiędzy tunelem a siecią lokalną
    WiFi.enableLongRange(true);//sprawdzić z i bez
    Serial.println("\nConnected!\nIP Address: " + WiFi.localIP());
    Serial.println("MAC Address:\n" + WiFi.macAddress());
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