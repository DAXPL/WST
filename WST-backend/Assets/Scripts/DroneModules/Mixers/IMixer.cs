using UnityEngine;
using WST.Communication;
namespace WST.Drone.Modules.Mixers
{
    public abstract class IMixer : ScriptableObject
    {
        public virtual void Loop(DroneControlData controllData, MotorDC[] motors) { }
    }
}