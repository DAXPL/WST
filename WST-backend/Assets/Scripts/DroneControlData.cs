using System.Runtime.InteropServices;

namespace WST.Communication
{
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