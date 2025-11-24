#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define VEHICLE_TYPE_BICOPTER

#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define UDP_CONTROLL_PORT 4210
#define MAX_ROGUE_TIME 1000

enum DroneStatus{
    WORKS,
    WARNING,
    BROKEN
};

#endif