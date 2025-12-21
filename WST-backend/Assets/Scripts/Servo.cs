using System.Collections;
using UnityEngine;

public class Servo : MonoBehaviour {
    [SerializeField] private Transform blades;
    [Range(-1f,1f), SerializeField] private float angle;
    [SerializeField] private float bladesRotationSpeed = 100;
    private Coroutine _rotateBladesCoroutine;

    [ContextMenu("Start Rotate Blades")]
    public void StartRotateBlades() {
        if (_rotateBladesCoroutine != null)
            return;

        if (blades == null) 
            throw new MissingComponentException("Blades transform is missing");

        _rotateBladesCoroutine = StartCoroutine(RotateBlades());
    } 
    
    [ContextMenu("Stop Rotate Blades")]
    public void StopRotateBlades() {
        if (_rotateBladesCoroutine == null) 
            return;
        
        StopCoroutine(_rotateBladesCoroutine);
        _rotateBladesCoroutine = null;
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