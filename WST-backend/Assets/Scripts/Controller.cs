using UnityEngine;
using UnityEngine.InputSystem;

namespace WST.Control {
    public class Controller : MonoBehaviour {
        [SerializeField] private float speed = 10;
        [SerializeField] private Vector3 moveDirection;

        // TODO: Change name of 'GetRealLifeMoveValues' method
        public void GetRealLifeMoveValues(InputAction.CallbackContext ctx) {
            if (ctx.performed) {
                moveDirection = ctx.ReadValue<Vector2>();
            }
        }

        public void GetVirtualMoveValues(Vector2 moveVector) {
            moveDirection = moveVector;
        }
    }
}