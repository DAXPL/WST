using UnityEngine;
using UnityEngine.InputSystem;

namespace WST.Control {
    public class Controller : MonoBehaviour {
        [SerializeField] private float speed = 10;
        [SerializeField] private float throttle;
        [SerializeField] private float yaw;

        // TODO: Change name of 'SetRealLifeMoveValues' method
        // public void SetRealLifeMoveValues(InputAction.CallbackContext ctx) {
        //     if (ctx.performed) {
        //         moveDirection = ctx.ReadValue<Vector2>();
        //     }
        // }

        public void SetVirtualThrottleValue(float throttleValue) {
            throttle = throttleValue;
        } 
        
        public void SetVirtualYawValue(float yawValue) {
            yaw = yawValue;
        }
    }
}