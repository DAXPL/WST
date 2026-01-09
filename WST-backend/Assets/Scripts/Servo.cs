using UnityEngine;

public class Servo : MonoBehaviour, IPWMInput {
    [SerializeField] private float maxRotation = 180;
    [Range(0, 1f)] private float _pwm;

    private void Update() {
        transform.rotation = Quaternion.Euler(0, 0, _pwm * maxRotation);
    }
    
    public void SetPWMSignal(int signal) {
        _pwm = signal;
    }
}