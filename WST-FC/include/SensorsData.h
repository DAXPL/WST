#ifndef SENSORSDATA_H
#define SENSORSDATA_H
#include <Arduino.h>
#pragma pack(push, 1)
struct SensorsData {
    int16_t pitch {0};
    int16_t roll {0};

    int16_t linearAccelX {0};
    int16_t linearAccelY {0};
    int16_t linearAccelZ {0};

    int16_t voltage {0};

    uint16_t distanceSensors[6];
    int16_t other[5];
};
#pragma pack(pop)
#endif