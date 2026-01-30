using UnityEngine;

public class ProximitySensor : MonoBehaviour, IDigitalSensor {
    [SerializeField] private float hitDistance = 10f;
    [SerializeField] private LayerMask hitMask;
    private Color _gizmoColor = Color.yellow;
    
    public object ReadValue() {
        if (Physics.Raycast(transform.position, Vector3.forward, out RaycastHit hit, hitDistance, hitMask)) {
            _gizmoColor = Color.green;
            return hit.distance;     
        }
        _gizmoColor = Color.red;
        return 0;
    }
    private void OnDrawGizmos()
    {
        Vector3 direction = transform.forward;
        Vector3 endPoint = transform.position + direction * hitDistance;

        Gizmos.color = _gizmoColor;
        Gizmos.DrawLine(transform.position, endPoint);
    }
}