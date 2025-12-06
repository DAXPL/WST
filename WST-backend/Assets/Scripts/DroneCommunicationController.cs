using System;
using System.Collections;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using UnityEngine;
using UnityEngine.InputSystem;
namespace WST.Communication
{
    public class DroneCommunicationController : MonoBehaviour
    {
        [Header("Input Settings")]
        public InputActionReference leftStickInput;
        public InputActionReference rightStickInput;

        [Header("Network Settings")]
        public string ipAddress = "192.168.0.49";
        public int port = 4210;

        private UdpClient udpClient;
        private IPEndPoint remoteEndPoint;

        [SerializeField] private Vector2 _leftStickVal;
        [SerializeField] private Vector2 _rightStickVal;

        void Start()
        {
            leftStickInput.action.Enable();
            rightStickInput.action.Enable();         
        }

        private void OnDestroy()
        {
            StopAllCoroutines();
            leftStickInput.action.Disable();
            rightStickInput.action.Disable();
            if (udpClient != null) udpClient.Close();
        }

        private void Update()
        {
            _leftStickVal = leftStickInput.action.ReadValue<Vector2>();
            _rightStickVal = rightStickInput.action.ReadValue<Vector2>();
        }

        [ContextMenu("Connect")]
        public void Connect()
        {
            udpClient = new UdpClient();
            remoteEndPoint = new IPEndPoint(IPAddress.Parse(ipAddress), port);
            StartCoroutine(ConnectionLoop());
        }

        private IEnumerator ConnectionLoop()
        {
            while (true)
            {
                DroneControlData data = new DroneControlData();
                data.throttle = (short)(_leftStickVal.y);
                data.yaw = (short)(_leftStickVal.x);
                data.pitch = (short)(_rightStickVal.y);
                data.roll = (short)(_rightStickVal.x);


                int size = Marshal.SizeOf(data);
                byte[] bytes = new byte[size];
                IntPtr ptr = Marshal.AllocHGlobal(size);

                try
                {
                    Marshal.StructureToPtr(data, ptr, true);
                    Marshal.Copy(ptr, bytes, 0, size);
                }
                finally
                {
                    Marshal.FreeHGlobal(ptr);
                }

                udpClient.Send(bytes, bytes.Length, remoteEndPoint);

                yield return new WaitForSeconds(0.05f); // 20 razy na sekundê (50ms)
            }
        }

        void OnApplicationQuit()
        {
            if (udpClient != null) udpClient.Close();
        }
    }
    [System.Serializable]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct DroneControlData
    {
        public short throttle;
        public short yaw;
        public short pitch;
        public short roll;
    }
}