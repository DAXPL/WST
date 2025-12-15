using UnityEngine;

public class GyroscopeSensor : MonoBehaviour, IDigitalSensor {
    [SerializeField] private Rigidbody droneRb;
    [SerializeField] private bool isInRadians;

    public object ReadValue() {
        if (droneRb != null) {
            if (isInRadians)
                return droneRb.angularVelocity * Mathf.Rad2Deg;
            
            return droneRb.angularVelocity;
        }

        Debug.LogError("Drone GameObject is missing");
        return Vector3.zero;
    }
}