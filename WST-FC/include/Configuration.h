#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define VEHICLE_TYPE_AIRBOAT

#define WIFI_SSID "Willson"
#define WIFI_PASSWORD "Autyzm2137"
#define UDP_CONTROLL_PORT 4210
#define MAX_ROGUE_TIME 1000
#define MIN_RSSI -80
#define COMMUNICATION_METHOD 0

enum DroneStatus
{
    WORKS,
    WARNING,
    BROKEN
};

#endif