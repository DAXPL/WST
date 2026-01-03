using System.Collections;
using UnityEngine;

public class Servo : MonoBehaviour {
    [SerializeField] private Transform blades;
    [Range(-1f,1f), SerializeField] private float angle;
    [SerializeField] private float bladesRotationSpeed = 100;
    private Coroutine _rotateBladesCoroutine;
    private Coroutine _decreaseBladesRotationSpeedCoroutine;
    
    [ContextMenu("Start Rotate Blades")]
    public void StartRotateBlades() {
        if (_rotateBladesCoroutine != null)
            return;

        if (blades == null) 
            throw new MissingComponentException("Blades transform is missing");

        _rotateBladesCoroutine = StartCoroutine(RotateBlades());
        
        if (_decreaseBladesRotationSpeedCoroutine == null)
            return;
        
        StopCoroutine(_decreaseBladesRotationSpeedCoroutine);
    } 
    
    [ContextMenu("Stop Rotate Blades")]
    public void StopRotateBlades() {
        if (_rotateBladesCoroutine == null) 
            return;
        
        StopCoroutine(_rotateBladesCoroutine);
        _rotateBladesCoroutine = null;
        
        if (_decreaseBladesRotationSpeedCoroutine == null)
            return;
        
        StopCoroutine(_decreaseBladesRotationSpeedCoroutine);
    }
    
    [ContextMenu("Slowly Stop Rotate Blades")]
    public void StopSlowlyRotateBlades() {
        if (_decreaseBladesRotationSpeedCoroutine != null)
            return;
        
        _decreaseBladesRotationSpeedCoroutine = StartCoroutine(DecreaseBladesRotationSpeed());
    }

    private IEnumerator DecreaseBladesRotationSpeed() {
        float defaultSpeed = bladesRotationSpeed;
        float step = defaultSpeed/3;
        
        while (bladesRotationSpeed > 0) {
            bladesRotationSpeed -= step * Time.deltaTime;
            yield return null;
        }

        StopRotateBlades();
        bladesRotationSpeed = defaultSpeed;
        _decreaseBladesRotationSpeedCoroutine = null;
    }

    private IEnumerator RotateBlades() {
        float step = 0;
        
        while (true) {
            step = (step + bladesRotationSpeed * Time.deltaTime) % 360f;
            Quaternion baseRotation = Quaternion.Euler(90f * angle, 0f, 0f);
            Quaternion spinRotation = Quaternion.AngleAxis(step, Vector3.up);
            
            blades.localRotation = baseRotation * spinRotation;
            yield return null;
        }
    }
}