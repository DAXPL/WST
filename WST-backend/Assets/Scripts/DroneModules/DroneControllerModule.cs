using UnityEngine;
using WST.Control;
namespace WST.Drone.Modules
{
    public class DroneControllerModule : MonoBehaviour, IDroneModule
    {
        [SerializeField] private Controller controller;
        private DroneManager _drone;
        public void Init(DroneManager drone)
        {
            _drone = drone;
        }

        public void Loop()
        {
            if(_drone == null || controller == null) return;
            _drone.controllData = controller.GetControls();
        }
    }
}