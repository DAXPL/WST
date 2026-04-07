using System;
using UnityEngine;
using WST.Communication;
namespace WST.Drone.Modules.Mixers
{
    [CreateAssetMenu(menuName = "ScriptableObjects/AirBoatMixer")]
    public class AirBoatMixer : IMixer
    {
        public override void Loop(DroneControlData controllData, MotorDC[] motors)
        {
            //"Differential Drive"
            short throttle = controllData.throttle;
            short yaw = controllData.yaw;

            short leftSpeed = (short)Mathf.Clamp(throttle + yaw, 0, 1000);
            short rightSpeed = (short)Mathf.Clamp(throttle - yaw, 0, 1000);

            if (motors.Length < 2)
            {
                Debug.LogWarning("Not enough motors for this mixer!");
                return;
            }
            if (motors[0]) motors[0].SetPWMSignal(rightSpeed);
            if (motors[1]) motors[1].SetPWMSignal(leftSpeed);
        }
    }
}