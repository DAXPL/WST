#ifndef SENSORSDATA_H
#define SENSORSDATA_H
#include <Arduino.h>
#pragma pack(push, 1)
struct SensorsData {
    float pitch {0};
    float roll {0};

    float voltage {0};

    uint16_t distanceSensors[6];
};
#pragma pack(pop)
#endif