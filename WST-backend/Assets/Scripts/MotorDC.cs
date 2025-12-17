using UnityEngine;

public class MotorDC : MonoBehaviour {
    [Range(0, 1f), SerializeField] private float pwm;
    [SerializeField] private float speed;
    [SerializeField] private Vector3 direction;
    [SerializeField] private Rigidbody droneRb;

    private void FixedUpdate() {
        droneRb.linearVelocity = (pwm * Time.fixedDeltaTime * speed) * direction;
    }
}