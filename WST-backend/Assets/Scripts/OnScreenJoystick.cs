using UnityEngine;
using UnityEngine.EventSystems;
using WST.Control;

public class OnScreenJoystick : MonoBehaviour, IDragHandler, IEndDragHandler {
    [SerializeField] private Controller controller;
    private Vector2 _startPosition;

    private void Start() {
        _startPosition = transform.position;
    }

    public void OnDrag(PointerEventData eventData) {
        Vector2 joystickOffset = eventData.position - _startPosition;
        Vector2 clampedPosition = Vector2.ClampMagnitude(joystickOffset, 150);
        transform.position = _startPosition + clampedPosition;
        
        controller.GetVirtualMoveValues(clampedPosition.normalized);
    }

    public void OnEndDrag(PointerEventData eventData) {
        transform.position = _startPosition;
    }
}