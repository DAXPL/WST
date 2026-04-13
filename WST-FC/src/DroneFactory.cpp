#include "DroneFactory.h"

#ifdef VEHICLE_TYPE_BICOPTER
  #include "BicopterMixer.h"
  #include "sensors/MpuSensor.h"
  MpuSensor mpuSensor;
#endif

#ifdef VEHICLE_TYPE_AIRBOAT
    #include "AirBoatMixer.h"
    #include "sensors/AdxlSensor.h"
    #include "modules/CameraModule.h"
    DCMotor motorL(16, 17, 4, 0); 
    DCMotor motorR(18, 19, 5, 1);
    AdxlSensor adxlSensor;
#endif

#ifdef VEHICLE_TYPE_TANK
    #include "AirBoatMixer.h"
    #include "modules/CameraModule.h"
    DCMotor motorL(13, 14, 0); 
    DCMotor motorR(15, 2, 1); 
#endif

IMixer* DroneFactory::BuildVehicle(SensorsModule& sensorsModule, std::vector<IModule*>& modules)
{
    IMixer* droneMixer = nullptr;
    
    #ifdef VEHICLE_TYPE_BICOPTER
        Serial.println("Configuring as BICOPTER");
        sensorsModule.AddSensor(&mpuSensor);
        droneMixer = new BicopterMixer();
    #endif
    
    #ifdef VEHICLE_TYPE_AIRBOAT
        Serial.println("Configuring as AIRBOAT");
        sensorsModule.AddSensor(&adxlSensor);
        droneMixer = new BoatMixer(&motorL, &motorR);
        modules.push_back(new CameraModule());
    #endif
    
    #ifdef VEHICLE_TYPE_TANK
        Serial.println("Configuring as TANK");
        droneMixer = new BoatMixer(&motorL, &motorR);
    #endif

    return droneMixer;
}