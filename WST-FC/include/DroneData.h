#ifndef DRONEDATA_H
#define DRONEDATA_H
#pragma pack(push, 1)
struct DroneControlData {
    int16_t throttle;
    int16_t yaw;
    int16_t pitch;
    int16_t roll;
};
#pragma pack(pop)
#endif