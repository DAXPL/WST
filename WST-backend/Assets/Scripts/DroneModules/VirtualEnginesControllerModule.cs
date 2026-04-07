using UnityEngine;
using WST.Drone.Modules.Mixers;
namespace WST.Drone.Modules
{
    public class VirtualEnginesControllerModule : MonoBehaviour, IDroneModule
    {
        private DroneManager _drone;
        [SerializeField] private IMixer mixer;
        [SerializeField] private MotorDC[] motors;
        public void Init(DroneManager drone)
        {
            _drone = drone;
        }

        public void Loop()
        {
            if (mixer == null) return;
            if (_drone == null) return;
            mixer.Loop(_drone.controllData,motors);
        }
    }
}
