using UnityEngine;

public class AccelerometerSensor : MonoBehaviour, IDigitalSensor {
    [SerializeField] private float noiseLevel = 0.02f;
    [SerializeField] private Rigidbody droneRb;
    
    public object ReadValue() {
        if (droneRb != null) {
            return droneRb.linearVelocity - (droneRb.linearVelocity * noiseLevel);
        }

        Debug.LogError("Drone GameObject is missing");
        return Vector3.zero;
    }
}