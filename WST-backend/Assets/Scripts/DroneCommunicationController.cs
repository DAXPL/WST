using System;
using System.Collections;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using UnityEngine;
using WST.Control;
namespace WST.Communication
{
    public class DroneCommunicationController : MonoBehaviour
    {
        [SerializeField] private Controller controller;
        [Header("Network Settings")]
        private string ipAddress = "192.168.0.39";
        private int port = 4210;
        [SerializeField] private float sendInterval = 1.0f/10.0f;

        private UdpClient _udpClient;
        private IPEndPoint _remoteEndPoint;

        [SerializeField] private SensorsData CurrentTelemetry;

        private void OnDestroy()
        {
            StopAllCoroutines();
            if (_udpClient != null) _udpClient.Close();
        }

        public void UpdateIP(string s)
        {
            ipAddress = s;
        }
        public void UpdatePort(string s)
        {
            int newPort = 0;
            if(Int32.TryParse(s, out newPort))
            {
                port = newPort;
            }
        }

        [ContextMenu("Connect")]
        public void Connect()
        {
            if (_udpClient != null) _udpClient.Close();
            _udpClient = new UdpClient();
            _remoteEndPoint = new IPEndPoint(IPAddress.Parse(ipAddress), port);
            StartCoroutine(ConnectionLoop());
            StartCoroutine(ReceiveLoop());
        }

        private IEnumerator ConnectionLoop()
        {
            while (true)
            {
                if (controller)
                {
                    DroneControlData data = controller.GetControls();

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

                    _udpClient.Send(bytes, bytes.Length, _remoteEndPoint);
                }
                yield return new WaitForSeconds(sendInterval);
            }
        }

        private IEnumerator ReceiveLoop()
        {
            while (true)
            {
                if (_udpClient != null && _udpClient.Available > 0)
                {
                    try
                    {
                        IPEndPoint source = new IPEndPoint(IPAddress.Any, 0);
                        byte[] receivedBytes = _udpClient.Receive(ref source);

                        if (receivedBytes.Length == Marshal.SizeOf(typeof(SensorsData)))
                        {
                            CurrentTelemetry = Deserialize<SensorsData>(receivedBytes);
                            Debug.Log($"Telemetry -> Pitch: {CurrentTelemetry.pitch/100.0f}, Voltage: {CurrentTelemetry.voltage}");
                        }
                    }
                    catch (Exception e)
                    {
                        Debug.LogError($"Receive Error: {e.Message}");
                    }
                }
                yield return null;
            }
        }
        private T Deserialize<T>(byte[] bytes) where T : struct
        {
            GCHandle handle = GCHandle.Alloc(bytes, GCHandleType.Pinned);
            try
            {
                return (T)Marshal.PtrToStructure(handle.AddrOfPinnedObject(), typeof(T));
            }
            finally
            {
                handle.Free();
            }
        }
        private void OnApplicationQuit()
        {
            if (_udpClient != null) _udpClient.Close();
        }
    }
}