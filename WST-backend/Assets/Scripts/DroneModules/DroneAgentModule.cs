using Unity.MLAgents;
using Unity.MLAgents.Actuators;
using Unity.MLAgents.Sensors;
using UnityEngine;
using WST.Drone.Modules;
namespace WST.Drone.Modules
{
    public class DroneAgentModule : Agent, IDroneModule
    {
        private DroneManager _drone;
        private Vector3 _startPosition;
        private Vector3 _startRotation;
        public void Init(DroneManager drone)
        {
            _drone = drone;
            _startPosition = this.transform.localPosition;
            _startRotation = this.transform.localEulerAngles;
        }

        public void Loop()
        {
            
        }

        public override void OnEpisodeBegin()
        {
            transform.localPosition = _startPosition;
            transform.localEulerAngles = _startRotation;
        }

        public override void CollectObservations(VectorSensor sensor)
        {
            if (_drone == null)
            {
                Debug.LogError("No drone manager!");
                return;
            }
            if(_drone.sensorsData.distanceSensors.Length < 6)
            {
                Debug.LogError("To little distanceSensors!!!");
                return;
            }
            
            sensor.AddObservation(_drone.sensorsData.pitch);
            sensor.AddObservation(_drone.sensorsData.roll);

            sensor.AddObservation(_drone.sensorsData.linearAccelX);
            sensor.AddObservation(_drone.sensorsData.linearAccelY);
            sensor.AddObservation(_drone.sensorsData.linearAccelZ);

            sensor.AddObservation(_drone.sensorsData.voltage);

            sensor.AddObservation(_drone.sensorsData.distanceSensors[0]);
            sensor.AddObservation(_drone.sensorsData.distanceSensors[1]);
            sensor.AddObservation(_drone.sensorsData.distanceSensors[2]);
            sensor.AddObservation(_drone.sensorsData.distanceSensors[3]);
            sensor.AddObservation(_drone.sensorsData.distanceSensors[4]);
            sensor.AddObservation(_drone.sensorsData.distanceSensors[5]);
        }

        public override void OnActionReceived(ActionBuffers actions)
        {
            if(_drone == null)
            {
                Debug.LogError("No drone manager!");
                return;
            }
            if (actions.ContinuousActions.Length < 4)
            {
                Debug.LogError("Not enough Continous Actions!");
                return;
            }
            
            _drone.controllData.throttle = (short)actions.ContinuousActions[0];
            _drone.controllData.yaw = (short)actions.ContinuousActions[1];
            _drone.controllData.pitch = (short)actions.ContinuousActions[2];
            _drone.controllData.roll = (short)actions.ContinuousActions[3];
        }
    
        public void GiveReward(float reward, bool endEpisode = false)
        {
            SetReward(reward);
            if(endEpisode) EndEpisode();
        }
    }
}