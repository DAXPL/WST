using System;
using UnityEngine;
using WST.Drone.Modules;
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
                ushort val = canReadFromSensor ?
                    Convert.ToUInt16(proximitySensors[i].ReadValue()) 
                    : (ushort)0;
                val *= 100;
                _drone.sensorsData.distanceSensors[i] = val;   
            }

        }
    }
}
