using UnityEngine;

[ RequireComponent(typeof(Rigidbody))]
public class MotorDC : MonoBehaviour, IPWMInput {
    [SerializeField] private float force = 1000;
    [Range(0, 1f)] private float _signal;
    private Rigidbody _rb;

    private void Awake() {
        _rb = GetComponent<Rigidbody>();
    }

    private void FixedUpdate() {
        _rb.AddForce(transform.up * (_signal * force * Time.fixedDeltaTime));
    }
    
    public void SetPWMSignal(int signal) {
        _signal = signal;
    }
}