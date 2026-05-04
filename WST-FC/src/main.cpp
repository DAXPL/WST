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
  Serial.println("Starting");
  Serial.flush();
  comms.Init();
  Serial.println("Comms initiated");
  Serial.flush();
  sensorsModule.Init();
  Serial.println("Sensors initiated");
  Serial.flush();
  droneMixer = DroneFactory::BuildVehicle(sensorsModule, modules);
  if(droneMixer != nullptr) 
    droneMixer->Init();
  Serial.println("Mixer initiated");
  Serial.flush();
  for (auto module : modules) 
    module->Init();
  Serial.println("Modules initiated");
  Serial.flush();
  Serial.println("Ok");
  Serial.flush();
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