using UnityEngine;
using System.Collections;

public class MotorDC : MonoBehaviour {
    [Range(0, 1f), SerializeField] private float pwm;
    [SerializeField] private float speed;
    [SerializeField] private Vector3 direction;
    [SerializeField] private Rigidbody droneRb;
    private Coroutine _startMovingCoroutine;
    
    [ContextMenu("Start Moving")]
    public void StartMoving() {
        if (_startMovingCoroutine != null)
            return;

        if (droneRb == null) 
            throw new MissingComponentException("Drone rigidbody is missing");
        
        _startMovingCoroutine = StartCoroutine(MoveMotor());
    } 
    
    [ContextMenu("Stop Moving")]
    public void StopMoving() {
        if (_startMovingCoroutine == null) 
            return;
        
        StopCoroutine(_startMovingCoroutine);
        _startMovingCoroutine = null;
        droneRb.linearVelocity = Vector3.zero;
    }
    
    private IEnumerator MoveMotor() {
        while (true) {
            droneRb.linearVelocity = (pwm * Time.fixedDeltaTime * speed) * direction;
            yield return new WaitForFixedUpdate();
        }
    }
}