#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define VEHICLE_TYPE_TANK

#define MAX_ROGUE_TIME 1000
#define MIN_RSSI -80
#define TELEMETRY_TIME 1000

#define USE_WIFI_UDP
//#define USE_ESP_NOW
#define USE_SERIAL
//#define USE_GAMEPAD

enum DroneStatus
{
    WORKS,
    WARNING,
    BROKEN
};

#define USE_WIREGUARD 0

#if USE_WIREGUARD
    #define WG_LOCAL_IP "10.0.0.2"
    #define WG_PRIVATE_KEY "KLUCZ_PRYWATNY_ESP32="
    #define WG_PEER_ADDRESS "198.51.100.1"
    #define WG_PEER_PORT 51820
    #define WG_PEER_PUBLIC_KEY "KLUCZ_PUBLICZNY_SERWERA="
#endif

#if defined(USE_WIFI_UDP) || defined(USE_ESP_NOW)
    #define WIFI_SSID "HPLOVER"
    #define WIFI_PASSWORD "Autyzm2137!"
    #define UDP_CONTROLL_PORT 4210
#endif

#endif