using System;
using Unity.MLAgents;
using Unity.MLAgents.Actuators;
using Unity.MLAgents.Sensors;
using UnityEngine;

public class AgentGoToGoal : Agent {
    [SerializeField] private float moveSpeed = 1;
    [SerializeField] private MeshRenderer floorMeshRenderer;
    [SerializeField] private Material winMaterial;
    [SerializeField] private Material loseMaterial;
    [SerializeField] private Transform goalTransform;

    public override void OnEpisodeBegin() {
        transform.localPosition = Vector3.zero;
    }
    
    public override void CollectObservations(VectorSensor sensor) {
        sensor.AddObservation(transform.position);
        sensor.AddObservation(goalTransform.position);
    }

    public override void OnActionReceived(ActionBuffers actions) {
        float moveX = actions.ContinuousActions[0];
        float moveZ = actions.ContinuousActions[1];
        
        transform.position += new Vector3(moveX, 0, moveZ) * Time.deltaTime * moveSpeed;
    }

    public override void Heuristic(in ActionBuffers actionsOut) {
        ActionSegment<float> continuousActions = actionsOut.ContinuousActions;
        continuousActions[0] = Input.GetAxisRaw("Horizontal");
        continuousActions[1] = Input.GetAxisRaw("Vertical");
    }

    private void OnTriggerEnter(Collider other) {
        if (other.TryGetComponent(out Goal goal)) {
            SetReward(10f);
            Debug.Log($"Goal reached! {StepCount}");
            floorMeshRenderer.material = winMaterial;
            EndEpisode();
        }
        
        if (other.TryGetComponent(out Wall wall)) {
            SetReward(-1f);
            floorMeshRenderer.material = loseMaterial;
            EndEpisode();
        }
        
        
    }
}
