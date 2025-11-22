#include <Arduino.h>
#include "DroneData.h"
#include "CommunicationModule.h"
#include "Configuration.h"

DroneControlData droneControllData;
DroneStatus connectionStatus {OK};
DroneStatus batteryStatus {OK};
DroneStatus droneStatus {OK};
CommunicationModule comms(&droneControllData, UDP_CONTROLL_PORT, &connectionStatus);
/*
TUDU:
Silniki krokowe 
Sterowanie PWM
*/

void setup() {
  Serial.begin(115200);
  comms.Init(); 
}

void loop() 
{
  comms.Loop();
}