using UnityEngine;

public class ProximitySensor : MonoBehaviour, IDigitalSensor {
    [SerializeField] private float hitDistance = 10f;
    [SerializeField] private LayerMask hitMask;
    
    public object ReadValue() {
        if (Physics.Raycast(transform.position, Vector3.forward, out RaycastHit hit, hitDistance, hitMask)) {
            return hit.distance;
        }
        
        return -1;
    }
}