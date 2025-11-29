#include <Arduino.h>
#include "DroneData.h"
#include "CommunicationModule.h"
#include "Configuration.h"
#include "SensorsModule.h"
#include "SensorsData.h"

#ifdef VEHICLE_TYPE_BICOPTER
  #include "sensors/MpuSensor.h"
  MpuSensor mpuSensor;
#endif

DroneControlData droneControllData{};
SensorsData sensorsData{};

DroneStatus connectionStatus{WORKS};
DroneStatus batteryStatus{WORKS};
DroneStatus droneStatus{WORKS};

CommunicationModule comms(&droneControllData, UDP_CONTROLL_PORT, &connectionStatus);
SensorsModule sensorsModule(&sensorsData);

void setup()
{
  Serial.begin(115200);

  #ifdef VEHICLE_TYPE_BICOPTER
    Serial.println("Configuring as BICOPTER");
    sensorsModule.AddSensor(&mpuSensor);
  #endif

  comms.Init();
  sensorsModule.Init();
}

void loop()
{
  comms.Loop();
  sensorsModule.Loop();

  Serial.printf("Pitch:%.2f,Roll:%.2f\n",
                sensorsData.pitch / 100.0f,
                sensorsData.roll / 100.0f);
}