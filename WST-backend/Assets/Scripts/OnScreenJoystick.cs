using UnityEngine;
using UnityEngine.EventSystems;

public class OnScreenJoystick : MonoBehaviour, IDragHandler, IEndDragHandler {
    [SerializeField] private MovementController movementController;
    private Vector2 _startPosition;

    private void Start() {
        _startPosition = transform.position;
    }

    public void OnDrag(PointerEventData eventData) {
        Vector2 joystickOffset = eventData.position - _startPosition;
        Vector2 clampedPosition = Vector2.ClampMagnitude(joystickOffset, 150);
        transform.position = _startPosition + clampedPosition;
        
        movementController.GetVirtualMoveValues(clampedPosition.normalized);
    }

    public void OnEndDrag(PointerEventData eventData) {
        transform.position = _startPosition;
    }
}