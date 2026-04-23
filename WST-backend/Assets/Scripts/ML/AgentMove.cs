using System.Collections;
using System.Collections.Generic;
using Unity.MLAgents;
using Unity.MLAgents.Actuators;
using Unity.MLAgents.Sensors;
using UnityEngine;

namespace MLAgent.Car {
    public class AgentMove : Agent {
        [Header("Agent Settings")]
        [SerializeField] private Vector2 moveSpeedRange;
        [SerializeField] private float rotateSpeed = 7;
        [SerializeField] private Vector2 agentSpawnAreaXRange;
        [SerializeField] private LayerMask layerMask;
        [SerializeField] private float carWidth;
        
        [Header("Map Generation")]
        [SerializeField] private byte obstaclesAmount; 
        [SerializeField] private Transform mapParent;
        [SerializeField] private GameObject obstaclePrefab;
        [SerializeField] private Vector2 obstacleSpawnAreaMin; // Min X and Z coordinates
        [SerializeField] private Vector2 obstacleSpawnAreaMax; // Max X and Z coordinates
        [SerializeField] private float obstacleYPosition; 
        
        private Rigidbody rb;
        private Quaternion startRotation;
        private Vector3 startPosition;
        private Vector3 lastPosition;
        private bool justReset = false;
        private float maxYawRate;
        private float moveSpeed;
        private List<GameObject> obstaclePool = new List<GameObject>();

        private void Start() {
            rb = GetComponent<Rigidbody>();

            CreatePrefabsPull();
            startRotation = transform.rotation;
            startPosition = transform.position;
            lastPosition = transform.position;
            StartCoroutine(CheckLastPosition());
        }

        private void FixedUpdate() {
            rb.linearVelocity = transform.forward * moveSpeed;
        }

        public override void OnEpisodeBegin() {
            transform.localPosition = RandomizePlayerXPosition();
            transform.rotation = startRotation;
            
            rb.linearVelocity = Vector3.zero;
            rb.angularVelocity = Vector3.zero;
            RandomizeObstaclesPosition();

            lastPosition = transform.position;
            
            moveSpeed = Random.Range(moveSpeedRange.x, moveSpeedRange.y);
            maxYawRate = moveSpeed * 0.2f;
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

        private void CreatePrefabsPull() {
            if (obstaclePrefab == null || obstaclesAmount == 0)
                return;

            for (int i = 0; i < obstaclesAmount; i++) {
                GameObject obstacle = Instantiate(
                    obstaclePrefab,
                    Vector3.one * -10,
                    Quaternion.identity,
                    mapParent
                );
                obstaclePool.Add(obstacle);
            }
        }

        private Vector3 RandomizePlayerXPosition() {
            float randomX = Random.Range(agentSpawnAreaXRange.x, agentSpawnAreaXRange.y);
            return new(randomX, startPosition.y, startPosition.z);
        }
        
        private void RandomizeObstaclesPosition() {
            if (obstaclePool.Count == 0) 
                CreatePrefabsPull();
    
            for (int i = 0; i < obstaclePool.Count; i++) {
                float randomX = Random.Range(obstacleSpawnAreaMin.x, obstacleSpawnAreaMax.x);
                float randomZ = Random.Range(obstacleSpawnAreaMin.y, obstacleSpawnAreaMax.y);

                if (obstaclePool[i] == null) 
                    return;
                
                obstaclePool[i].transform.position = new Vector3(randomX, obstacleYPosition, randomZ);
            }
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

                float yawRate = Mathf.Abs(rb.angularVelocity.y);
                float distancedMoved = Vector3.Distance(lastPosition, transform.position);
                // float angle = Vector3.SignedAngle(lastForward, transform.forward, Vector3.up);

                AddReward(distancedMoved * 0.5f);

                if (yawRate > maxYawRate) {
                    AddReward(-1f * distancedMoved);
                    EndEpisode();
                }

                // if (Mathf.Abs(angle) > 50) {
                //     AddReward(-1f * distancedMoved);
                //     EndEpisode();
                // }
            }
        }
    }
}