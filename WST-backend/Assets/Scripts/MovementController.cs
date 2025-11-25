using UnityEngine;
using UnityEngine.InputSystem;

public class MovementController : MonoBehaviour {
    [SerializeField] private float speed;
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