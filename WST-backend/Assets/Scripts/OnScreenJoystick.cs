using UnityEngine;
using UnityEngine.EventSystems;

enum JoystickAxis {
    Horizontal,
    Vertical
}

namespace WST.Control {
    public class OnScreenJoystick : MonoBehaviour, IDragHandler, IEndDragHandler {
        [SerializeField, Tooltip("If true, joystick returns to the starting position on end drag")]
        private bool onEndDragReturn;

        [SerializeField, Tooltip("Axis in which the slider is allowed to move")]
        private JoystickAxis axis;

        private Vector2 _startPosition;
        private VirtualController _virtualController;

        private void Start() {
            _startPosition = transform.position;
            _virtualController = GetComponentInParent<VirtualController>();
        }

        public void OnDrag(PointerEventData eventData) {
            Vector2 joystickOffset = eventData.position - _startPosition;
            Vector2 clampedPosition = Vector2.ClampMagnitude(joystickOffset, 150);
            Vector2 joystickPosition = _startPosition + clampedPosition;

            switch (axis) {
                case JoystickAxis.Horizontal:
                    _virtualController.GetController.SetVirtualYawValue(clampedPosition.normalized.x);
                    joystickPosition.y = _startPosition.y;
                break;

                case JoystickAxis.Vertical:
                    _virtualController.GetController.SetVirtualThrottleValue(clampedPosition.normalized.y);
                    joystickPosition.x = _startPosition.x;
                break;
            }

            transform.position = joystickPosition;
        }

        public void OnEndDrag(PointerEventData eventData) {
            if (!onEndDragReturn)
                return;

            transform.position = _startPosition;

            switch (axis) {
                case JoystickAxis.Horizontal:
                    _virtualController.GetController.SetVirtualYawValue(0);
                break;

                case JoystickAxis.Vertical:
                    _virtualController.GetController.SetVirtualThrottleValue(0);
                break;
            }
        }
    }
}
