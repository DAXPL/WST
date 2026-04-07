using UnityEngine;

public class ProximitySensor : MonoBehaviour, IDigitalSensor {
    [SerializeField] private float hitDistance = 10f;
    [SerializeField] private float curhitDistance = 0f;
    [SerializeField] private LayerMask hitMask;
    private Color _gizmoColor = Color.yellow;

    [ContextMenu("Read value")]
    public object ReadValue() {
        if (Physics.Raycast(transform.position, transform.forward, out RaycastHit hit, hitDistance, hitMask)) {
            _gizmoColor = Color.green;
            return hit.distance;     
        }
        _gizmoColor = Color.red;
        return hitDistance;
    }
    private void OnDrawGizmos()
    {
        Vector3 endPoint = transform.position + transform.forward * hitDistance;

        Gizmos.color = _gizmoColor;
        Gizmos.DrawLine(transform.position, endPoint);
    }
}