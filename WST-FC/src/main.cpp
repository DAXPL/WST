#include <Arduino.h>
#include "DroneData.h"
#include "CommunicationModule.h"
#include "Configuration.h"
#include "SensorsModule.h"
#include "SensorsData.h"

DroneControlData droneControllData {};
SensorsData sensorsData {};

DroneStatus connectionStatus {WORKS};
DroneStatus batteryStatus {WORKS};
DroneStatus droneStatus {WORKS};

CommunicationModule comms(&droneControllData, UDP_CONTROLL_PORT, &connectionStatus);
SensorsModule sens(&sensorsData);

void setup() {
  Serial.begin(115200);
  comms.Init(); 
  sens.Init();
}

void loop() 
{
  comms.Loop();
  sens.Loop();

  Serial.printf("Pitch:%.2f,Roll:%.2f\n",sensorsData.pitch,sensorsData.roll);
}