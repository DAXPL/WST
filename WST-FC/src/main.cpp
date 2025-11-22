#include <Arduino.h>
#include "DroneData.h"
#include "CommunicationModule.h"
#include "Configuration.h"

DroneControlData droneControllData;
CommunicationModule comms(&droneControllData, UDP_CONTROLL_PORT);
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
  Serial.print("Throttle"+droneControllData.throttle);
}