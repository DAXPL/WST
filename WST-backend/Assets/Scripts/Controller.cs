using UnityEngine;
using UnityEngine.InputSystem;
using WST.Communication;

namespace WST.Control {
    public class Controller : MonoBehaviour {

        [Header("Input Settings")]
        [SerializeField] private InputActionReference leftStickInput;
        [SerializeField] private InputActionReference rightStickInput;

        private float _virtualThrottle;
        private float _virtualYaw;
        private float _virtualPitch;
        private float _virtualRoll;

        private const float Multiplier = 1000;

        private DroneControlData _controllData;
        
        // TODO: Change name of 'SetRealLifeMoveValues' method
        // public void SetRealLifeMoveValues(InputAction.CallbackContext ctx) {
        //     if (ctx.performed) {
        //         moveDirection = ctx.ReadValue<Vector2>();
        //     }
        // }

        public void Update()
        {
            Vector2 leftStickVal  = Vector2.zero;
            Vector2 rightStickVal = Vector2.zero;

            leftStickVal   = leftStickInput.action.ReadValue<Vector2>();
            rightStickVal  = rightStickInput.action.ReadValue<Vector2>();

            float finalThrottle = GetStrongerInput(leftStickVal.y, _virtualThrottle);
            float finalYaw      = GetStrongerInput(leftStickVal.x, _virtualYaw);
            float finalPitch    = 0;
            float finalRoll     = 0;

            _controllData.throttle = (short)(finalThrottle * Multiplier);
            _controllData.yaw      = (short)(finalYaw * Multiplier);
            _controllData.pitch    = (short)(finalPitch * Multiplier);
            _controllData.roll     = (short)(finalRoll * Multiplier);
        }

        public void SetVirtualThrottleValue(float throttleValue) {
            _virtualThrottle = throttleValue;
        } 
        
        public void SetVirtualYawValue(float yawValue) {
            _virtualYaw = yawValue;
        }

        // This struct fits into 8 bytes.
        // Copying is trivial for the CPU and ensures data atomicity without needing locks.
        public DroneControlData GetControls()
        {
            return _controllData;
        }

        private float GetStrongerInput(float a, float b)
        {
            return (Mathf.Abs(a) > Mathf.Abs(b)) ? a : b;
        }
    }
}