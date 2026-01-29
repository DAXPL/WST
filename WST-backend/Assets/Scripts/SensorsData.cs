using System.Runtime.InteropServices;
using UnityEngine;

[System.Serializable]
public struct SensorsData
{
    public short pitch;
    public short roll;

    public short linearAccelX;
    public short linearAccelY;
    public short linearAccelZ;

    public short voltage;

    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
    public ushort[] distanceSensors;

    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 5)]
    public short[] other;
}
