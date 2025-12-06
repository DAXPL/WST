using UnityEngine;

namespace WST.Control {
    public class VirtualController : MonoBehaviour {
        [SerializeField] private Controller controller;

        public Controller GetController => controller;
    }
}