#include <Arduino.h>
#include "DroneData.h"
#include "CommunicationModule.h"
#include "Configuration.h"
#include "SensorsModule.h"
#include "SensorsData.h"
#include "modules/IModule.h"

#ifdef VEHICLE_TYPE_BICOPTER
  #include "BicopterMixer.h"
  #include "sensors/MpuSensor.h"
  MpuSensor mpuSensor;
#endif

#ifdef VEHICLE_TYPE_AIRBOAT
    #include "AirBoatMixer.h"
    //#include "sensors/AdxlSensor.h"
    #include "modules/CameraModule.h"
    //Esp wrom
    DCMotor motorL(16, 17, 4, 0); 
    DCMotor motorR(18, 19, 5, 1);
    //AdxlSensor adxlSensor;
    //esp cam
    //DCMotor motorL(13, 14, 0); 
    //DCMotor motorR(15, 2, 1);
    
#endif

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
  
  #ifdef VEHICLE_TYPE_BICOPTER
    Serial.println("Configuring as BICOPTER");
    sensorsModule.AddSensor(&mpuSensor);
    droneMixer = new BicopterMixer();
  #endif
  #ifdef VEHICLE_TYPE_AIRBOAT
    Serial.println("Configuring as AIRBOAT");
    droneMixer = new BoatMixer(&motorL, &motorR);
  #endif

  comms.Init();
  sensorsModule.Init();
  if(droneMixer != nullptr) droneMixer->Init();
  for (auto module : modules)
  {
    module->Init();
  }
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
  if(droneMixer != nullptr) droneMixer->Update(&droneControllData, &sensorsData);

  for (auto module : modules)
  {
    module->Loop(&comms, &sensorsData);
  }
}