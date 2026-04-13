#include <Arduino.h>
#include "DroneData.h"
#include "CommunicationModule.h"
#include "Configuration.h"
#include "SensorsModule.h"
#include "SensorsData.h"
#include "modules/IModule.h"
#include "IMixer.h"
#include "DroneFactory.h"

DroneControlData droneControllData{};
SensorsData sensorsData{};

DroneStatus connectionStatus{WORKS};
DroneStatus batteryStatus{WORKS};
DroneStatus droneStatus{WORKS};

CommunicationModule comms(&droneControllData, &connectionStatus);
SensorsModule sensorsModule(&sensorsData);
std::vector<IModule *> modules;

IMixer* droneMixer = nullptr;
unsigned long lastTelemetryTimestamp {0};

void setup()
{
  Serial.begin(115200);
  
  comms.Init();
  sensorsModule.Init();
  droneMixer = DroneFactory::BuildVehicle(sensorsModule, modules);
  if(droneMixer != nullptr) 
    droneMixer->Init();
  for (auto module : modules) 
    module->Init();
}

void loop()
{
  comms.Loop();
  sensorsModule.Loop();
  if(millis() - lastTelemetryTimestamp > TELEMETRY_TIME)
  {
    lastTelemetryTimestamp = millis();
    comms.SendData(&sensorsData);
  } 

  if(droneMixer != nullptr) 
    droneMixer->Update(&droneControllData, &sensorsData);

  for (auto module : modules)
    module->Loop(&comms, &sensorsData);
}