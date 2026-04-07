using System;
using UnityEngine;
namespace WST.Drone.Modules
{
    public class VirtualSensorsReadingsModule : MonoBehaviour, IDroneModule
    {
        private DroneManager _drone;
        [SerializeField] private GyroscopeSensor gyroscopeSensor;
        [SerializeField]
        private ProximitySensor[] proximitySensors;
        public void Init(DroneManager drone)
        {
            _drone = drone;
        }

        public void Loop()
        {
            if(_drone == null) return;
            if (gyroscopeSensor)
            {
                Vector3 gyroAccelVal = (Vector3)gyroscopeSensor.ReadValue();

                _drone.sensorsData.linearAccelX = (short) gyroAccelVal.x;
                _drone.sensorsData.linearAccelY = (short) gyroAccelVal.y;
                _drone.sensorsData.linearAccelZ = (short) gyroAccelVal.z;
            }
            for (int i = 0; i < _drone.sensorsData.distanceSensors.Length; i++) 
            {
                
                bool canReadFromSensor = i < proximitySensors.Length && proximitySensors[i] != null;
                float distance = canReadFromSensor ?
                    (float)proximitySensors[i].ReadValue()
                    :0.0f;
                distance *= 100;//to cm
                if (distance < 0) distance = 0;
                ushort val = (ushort)distance;
                _drone.sensorsData.distanceSensors[i] = val;
                
            }

        }
    }
}
