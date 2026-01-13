using UnityEngine;

// Connect it with a joint
[RequireComponent(typeof(Rigidbody))]
public class GyroscopeSensor : MonoBehaviour, IDigitalSensor {
    [SerializeField] private bool isInRadians;
    private Rigidbody _rb;

    private void Awake() {
        _rb = GetComponent<Rigidbody>();
    }
    
    public object ReadValue() {
        if (_rb != null) {
            if (isInRadians)
                return _rb.angularVelocity * Mathf.Rad2Deg;
            
            return _rb.angularVelocity;
        }

        Debug.LogError("Rigidbody component is missing");
        return Vector3.zero;
    }
}