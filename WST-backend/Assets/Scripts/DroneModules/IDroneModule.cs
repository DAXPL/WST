using UnityEngine;
namespace WST.Drone.Modules
{
    public interface IDroneModule
    {
        public void Init(DroneManager drone);
        public void Loop();
    }
}