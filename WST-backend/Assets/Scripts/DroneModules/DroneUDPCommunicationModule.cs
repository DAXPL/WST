using System;
using System.Collections;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using UnityEngine;
using WST.Drone;

namespace WST.Drone.Modules
{
    public class DroneUDPCommunicationModule : MonoBehaviour, IDroneModule
    {
        public string ipAddress = "192.168.0.39";
        public int port = 4210;
        public float tickRate = 1.0f / 10.0f;

        private DroneManager _drone;
        private UdpClient _udpClient;
        private IPEndPoint _remoteEndPoint;
        public void Init(DroneManager drone)
        {
            _drone = drone;
            if(tickRate <= 0.0f )
            {
                Debug.LogError($"Tick rate too low! {tickRate} s!");
                tickRate = 1.0f;
                Debug.LogError($"New tickrate set at {tickRate} s!");
            }
        }
        public void Loop()
        {
            if (_drone == null) return;
        }
        public void UpdateIP(string s)
        {
            ipAddress = s;
        }
        public void UpdatePort(string s)
        {
            int newPort = 0;
            if (Int32.TryParse(s, out newPort))
            {
                port = newPort;
            }
        }

        [ContextMenu("Connect")]
        public void Connect()
        {
            Disconnect();
            _udpClient = new UdpClient();
            _remoteEndPoint = new IPEndPoint(IPAddress.Parse(ipAddress), port);
            StartCoroutine(SendLoop());
            StartCoroutine(ReceiveLoop());
        }
        
        [ContextMenu("Disconnect")]
        public void Disconnect()
        {
            if (_udpClient != null) _udpClient.Close();
        }
        
        private IEnumerator SendLoop()
        {
            while (true)
            {
                if (_drone != null)
                {
                    yield return new WaitForSeconds(tickRate);
                
                    int size = Marshal.SizeOf(_drone.controllData);
                    byte[] bytes = new byte[size];
                    IntPtr ptr = Marshal.AllocHGlobal(size);

                    try
                    {
                        Marshal.StructureToPtr(_drone.controllData, ptr, true);
                        Marshal.Copy(ptr, bytes, 0, size);
                    }
                    finally
                    {
                        Marshal.FreeHGlobal(ptr);
                    }

                    _udpClient.Send(bytes, bytes.Length, _remoteEndPoint);
                }
                yield return new WaitForSeconds(tickRate);
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

                        if (_drone != null && receivedBytes.Length == Marshal.SizeOf(typeof(SensorsData)))
                        {
                            _drone.sensorsData = Deserialize<SensorsData>(receivedBytes);
                        }
                    }
                    catch (Exception e)
                    {
                        Debug.LogError($"Receive Error: {e.Message}");
                    }
                }
                yield return new WaitForSeconds(tickRate);
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
            Disconnect();
        }
    }
}