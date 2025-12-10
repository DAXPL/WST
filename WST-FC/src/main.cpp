#include <Arduino.h>
#include "DroneData.h"
#include "CommunicationModule.h"
#include "Configuration.h"
#include "SensorsModule.h"
#include "SensorsData.h"

#ifdef VEHICLE_TYPE_BICOPTER
  #include "BicopterMixer.h"
  #include "sensors/MpuSensor.h"
  MpuSensor mpuSensor;
#endif

#ifdef VEHICLE_TYPE_AIRBOAT
    #include "AirBoatMixer.h"
    DCMotor motorL(16, 17, 4, 0); 
    DCMotor motorR(18, 19, 5, 1);
#endif

DroneControlData droneControllData{};
SensorsData sensorsData{};

DroneStatus connectionStatus{WORKS};
DroneStatus batteryStatus{WORKS};
DroneStatus droneStatus{WORKS};

CommunicationModule comms(&droneControllData, UDP_CONTROLL_PORT, &connectionStatus);
SensorsModule sensorsModule(&sensorsData);

IMixer* droneMixer = nullptr;

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
}

void loop()
{
  comms.Loop();
  sensorsModule.Loop();
  if(droneMixer != nullptr) droneMixer->Update(&droneControllData, &sensorsData);
}