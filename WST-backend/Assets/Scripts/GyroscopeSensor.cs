using UnityEngine;

public class GyroscopeSensor : MonoBehaviour, IDigitalSensor {
    [SerializeField] private Rigidbody droneRb;

    public object ReadValue() {
        if (droneRb != null) 
            return droneRb.angularVelocity * Mathf.Rad2Deg;
        
        Debug.LogError("Drone GameObject is missing");
        return Vector3.zero;
    }
}