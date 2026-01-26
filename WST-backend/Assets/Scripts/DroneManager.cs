using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using WST.Communication;
using WST.Drone.Modules;
namespace WST.Drone 
{
    public class DroneManager : MonoBehaviour
    {
        public DroneControlData controllData;
        public SensorsData sensorsData;
        private List<IDroneModule> _modules = new List<IDroneModule>();

        private void Start()
        {
            _modules = gameObject.GetComponents<IDroneModule>().ToList<IDroneModule>();
            foreach (var module in _modules) module.Init(this);
        }
        private void Update()
        {
            foreach (var module in _modules) module.Loop();
        }
        public void AddModule(IDroneModule module)
        {
            module.Init(this);
            _modules.Add(module);
        }
    }
}