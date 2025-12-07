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
        [SerializeField] private float sendInterval = 0.1f;

        private UdpClient udpClient;
        private IPEndPoint remoteEndPoint;

        private void OnDestroy()
        {
            StopAllCoroutines();
            if (udpClient != null) udpClient.Close();
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
            if (udpClient != null) udpClient.Close();
            udpClient = new UdpClient();
            remoteEndPoint = new IPEndPoint(IPAddress.Parse(ipAddress), port);
            StartCoroutine(ConnectionLoop());
        }

        private IEnumerator ConnectionLoop()
        {
            while (true)
            {
                if (controller)
                {
                    DroneControlData data = controller.GetControlls();

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
                }
                yield return new WaitForSeconds(sendInterval);
            }
        }

        void OnApplicationQuit()
        {
            if (udpClient != null) udpClient.Close();
        }
    }
}