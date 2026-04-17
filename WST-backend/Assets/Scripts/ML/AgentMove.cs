using System.Collections;
using Unity.MLAgents;
using Unity.MLAgents.Actuators;
using Unity.MLAgents.Sensors;
using UnityEngine;

namespace MLAgent.Car {
    public class AgentMove : Agent {
        [SerializeField] private float moveSpeed = 5;
        [SerializeField] private float rotateSpeed = 7;
        [SerializeField] private LayerMask layerMask;
        [SerializeField] private float carWidth;
        private Rigidbody rb;
        private Quaternion startRotation;
        private Vector3 startPosition;
        private Vector3 lastPosition;
        private bool justReset = false;

        private void Start() {
            rb = GetComponent<Rigidbody>();

            startRotation = transform.rotation;
            startPosition = transform.position;
            lastPosition = transform.position;
            StartCoroutine(CheckLastPosition());
        }

        private void FixedUpdate() {
            rb.linearVelocity = transform.forward * moveSpeed;
        }

        public override void OnEpisodeBegin() {
            transform.localPosition = startPosition;
            transform.rotation = startRotation;

            rb.linearVelocity = Vector3.zero;
            rb.angularVelocity = Vector3.zero;

            lastPosition = transform.position;
            justReset = true;
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
                }
                else {
                    sensor.AddObservation(1f);
                }
            }
        }

        public override void OnActionReceived(ActionBuffers actions) {
            float rotate = actions.ContinuousActions[0];
   
            // Differential wheel forces for turning
            float leftWheelForce = rotate * rotateSpeed;
            float rightWheelForce = -rotate * rotateSpeed;

            Vector3 leftWheelPos = transform.position + (-transform.right * carWidth);
            Vector3 rightWheelPos = transform.position + (transform.right * carWidth);

            rb.AddForceAtPosition(transform.forward * leftWheelForce, leftWheelPos);
            rb.AddForceAtPosition(transform.forward * rightWheelForce, rightWheelPos);
        }


        private IEnumerator CheckLastPosition() {
            while (true) {
                lastPosition = transform.position;
                Vector3 lastForward = transform.forward;

                yield return new WaitForSeconds(0.5f);

                // skip this cycle completely if the episode just ended
                if (justReset) {
                    justReset = false;
                    continue;
                }

                float distancedMoved = Vector3.Distance(lastPosition, transform.position);
                float angle = Vector3.SignedAngle(lastForward, transform.forward, Vector3.up);

                AddReward(distancedMoved * 0.5f);

                if (Mathf.Abs(angle) > 50) {
                    AddReward(-1f * distancedMoved);
                    EndEpisode();
                }
            }
        }

        public override void Heuristic(in ActionBuffers actionsOut) {
            ActionSegment<float> continuousActions = actionsOut.ContinuousActions;
            continuousActions[0] = Input.GetAxisRaw("Horizontal");
        }

        private void OnCollisionEnter(Collision other) {
            if (other.gameObject.TryGetComponent(out Obstacle obstacle)) {
                AddReward(-20f);
                EndEpisode();
            }
        }
    }
}