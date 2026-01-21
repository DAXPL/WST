using UnityEngine;
using WST.Control;
namespace WST.Drone.Modules
{
    public class DroneControllerModule : MonoBehaviour, IDroneModule
    {
        [SerializeField] private Controller _controller;
        private DroneManager _drone;
        public void Init(DroneManager drone)
        {
            _drone = drone;
        }

        public void Loop()
        {
            if(_drone == null || _controller == null) return;
            _drone.controllData = _controller.GetControls();
        }
    }
}