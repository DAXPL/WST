using UnityEngine;
using WST.Communication;
namespace WST.Drone 
{
    public class DroneManager : MonoBehaviour
    {
        //drone manager <-> drone communication tunnel<->external ip or gameobject
        public DroneControlData controllData;
        public SensorsData sensorsData;


    }
}