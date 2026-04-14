using System;
using System.Collections;
using ML.Template;
using Unity.MLAgents;
using Unity.MLAgents.Actuators;
using Unity.MLAgents.Sensors;
using UnityEngine;

namespace MLAgent.Car {
    public class AgentMove : Agent {
        [SerializeField] private float moveSpeed = 1;
        [SerializeField] private float rotateSpeed = 2;
        //[SerializeField] private Transform goalTransform;
        [SerializeField] private LayerMask layerMask;
        [SerializeField] private float carWidth;
        //[SerializeField] private int maxSteps = 5000;
        [SerializeField] private Vector3[] goalPositions;
        private Vector3 _agentLeftCorner;
        private Vector3 _agentRightCorner;
        private Rigidbody rb;
        //private int _goalPositionsIndex = 0;
        private Quaternion startRotation;
        private Vector3 startPosition;
        Vector3 lastPosition;
        float totalDistance = 0f;
        
        private void Start() {
            rb = GetComponent<Rigidbody>();
            
            startRotation = transform.rotation;
            startPosition = transform.position;
            lastPosition = transform.position;
            StartCoroutine(CheckLastPosition());
        }

        public override void OnEpisodeBegin() {
            totalDistance = 0f;
            transform.localPosition = startPosition;
            transform.rotation = startRotation;
            rb.linearVelocity = Vector3.zero;
            rb.angularVelocity = Vector3.zero;
        }

        public override void CollectObservations(VectorSensor sensor) {
            sensor.AddObservation(transform.position);
            sensor.AddObservation(rb.linearVelocity);
            sensor.AddObservation(rb.angularVelocity);

            // Raycast observations 
            for (int i = -1; i <= 2; i += 2) {
                float angle = i * 15f;
                Vector3 direction = Quaternion.Euler(0, angle, 0) * transform.forward;
                if (Physics.Raycast(transform.position, direction, out RaycastHit hit, 20f, layerMask)) {
                    sensor.AddObservation(hit.distance / 20f);
                } else {
                    sensor.AddObservation(1f);
                }
            }
        }

        public override void OnActionReceived(ActionBuffers actions) {
            float rotate = actions.ContinuousActions[0];
            float move = actions.ContinuousActions[1];

            // Penalty for going backward
            if (move < 0) {
                AddReward(-1f);
                EndEpisode();
            }

            if (Mathf.Abs(rb.linearVelocity.magnitude) > 0.5f) {
                AddReward(0.001f);
            }
            else {
                AddReward(-0.0001f);
            }

            // Calculate left and right wheel forces
            float leftWheelForce = move + (rotate * rotateSpeed);
            float rightWheelForce = move - (rotate * rotateSpeed);

            // Define wheel positions
            Vector3 leftWheelPos = transform.position + (-transform.right * carWidth);
            Vector3 rightWheelPos = transform.position + (transform.right * carWidth);

            // Apply forces at wheel positions
            rb.AddForceAtPosition(transform.forward * leftWheelForce * moveSpeed, leftWheelPos);
            rb.AddForceAtPosition(transform.forward * rightWheelForce * moveSpeed, rightWheelPos);
        }

        private IEnumerator CheckLastPosition() {
            while (true) {
                lastPosition = transform.position;
                yield return new WaitForSeconds(0.5f);
                float distancedMoved = Vector3.Distance(lastPosition, transform.position);
                // Always reward distance traveled
                AddReward(distancedMoved * 0.5f);
        
                // Penalty if stuck/not moving enough
                if (distancedMoved < 0.5f) {
                    AddReward(-0.3f);
                }
                
                // if (distancedMoved > 2f) {
                //      AddReward(1 * distancedMoved);
                // } else {
                //     // if (distancedMoved <= 0) {
                //     //     distancedMoved = 0.00001f;
                //     // }
                //     //
                //     // AddReward(-0.1f / distancedMoved);
                //     AddReward(-0.1f);
                // }
            }
        }

        public override void Heuristic(in ActionBuffers actionsOut) {
            ActionSegment<float> continuousActions = actionsOut.ContinuousActions;
            continuousActions[0] = Input.GetAxisRaw("Horizontal");
            continuousActions[1] = Input.GetAxisRaw("Vertical");
        }

        private void OnCollisionEnter(Collision other) {
            if (other.gameObject.TryGetComponent(out Obstacle obstacle)) {
                AddReward(-20f);
                EndEpisode();
            }
        }
    }
}