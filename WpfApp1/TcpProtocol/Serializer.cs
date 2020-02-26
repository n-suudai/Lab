using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Net.Sockets;
using System.Text;

namespace TcpProtocol
{
    /*
     * string, byte[] => data length(4byte) + byte[]
     * others         => sizeof(x)
     */
    public class Serializer
    {
        Stream Stream { get; set; }

        public Serializer(Stream stream)
        {
            Stream = stream;
        }

        public void WriteByte(byte value)
        {
            WriteByteRaw(value);
        }

        public void WriteSByte(sbyte value)
        {
            WriteByte((byte)value);
        }

        public void WriteString(string value)
        {
            WriteString(value, Encoding.ASCII);
        }

        public void WriteString(string value, Encoding encoding)
        {
            byte[] data = encoding.GetBytes(value);

            // data length
            WriteInt(data.Length);

            // data
            WriteRaw(data);
        }

        public void WriteShort(short value)
        {
            byte[] data = BitConverter.GetBytes(value);
            WriteRaw(data);
        }

        public void WriteUShort(ushort value)
        {
            byte[] data = BitConverter.GetBytes(value);
            WriteRaw(data);
        }

        public void WriteInt(int value)
        {
            byte[] data = BitConverter.GetBytes(value);
            WriteRaw(data);
        }

        public void WriteUInt(uint value)
        {
            byte[] data = BitConverter.GetBytes(value);
            WriteRaw(data);
        }

        public void WriteLong(long value)
        {
            byte[] data = BitConverter.GetBytes(value);
            WriteRaw(data);
        }

        public void WriteULong(ulong value)
        {
            byte[] data = BitConverter.GetBytes(value);
            WriteRaw(data);
        }

        public void WriteFloat(float value)
        {
            byte[] data = BitConverter.GetBytes(value);
            WriteRaw(data);
        }

        public void WriteDouble(double value)
        {
            byte[] data = BitConverter.GetBytes(value);
            WriteRaw(data);
        }

        public void WriteBytes(byte[] data)
        {
            // data length
            WriteInt(data.Length);

            // data
            WriteRaw(data);
        }

        private void WriteRaw(byte[] data)
        {
            Stream.Write(data, 0, data.Length);
        }

        private void WriteByteRaw(byte value)
        {
            Stream.WriteByte(value);
        }

        public byte ReadByte()
        {
            byte value = (byte)ReadByteRaw();

            return value;
        }

        public sbyte ReadSByte()
        {
            sbyte value = (sbyte)ReadByte();
            
            return value;
        }

        public string ReadString()
        {
            return ReadString(Encoding.ASCII);
        }

        public string ReadString(Encoding encoding)
        {
            // data length
            int length = ReadInt();

            // data
            byte[] data = ReadRaw(length);

            if (data == null) { return ""; }

            string value = encoding.GetString(data);

            return value;
        }

        public short ReadShort()
        {
            byte[] data = ReadRaw(sizeof(short));

            if (data == null) { return 0; }

            short value = BitConverter.ToInt16(data);

            return value;
        }

        public ushort ReadUShort()
        {
            byte[] data = ReadRaw(sizeof(ushort));

            if (data == null) { return 0; }

            ushort value = BitConverter.ToUInt16(data);

            return value;
        }

        public int ReadInt()
        {
            byte[] data = ReadRaw(sizeof(int));

            if (data == null) { return 0; }

            int value = BitConverter.ToInt32(data);

            return value;
        }

        public uint ReadUInt()
        {
            byte[] data = ReadRaw(sizeof(uint));

            if (data == null) { return 0; }

            uint value = BitConverter.ToUInt32(data);

            return value;
        }

        public long ReadLong()
        {
            byte[] data = ReadRaw(sizeof(long));

            if (data == null) { return 0; }

            long value = BitConverter.ToInt64(data);

            return value;
        }

        public ulong ReadULong()
        {
            byte[] data = ReadRaw(sizeof(ulong));

            if (data == null) { return 0; }

            ulong value = BitConverter.ToUInt64(data);

            return value;
        }

        public float ReadFloat()
        {
            byte[] data = ReadRaw(sizeof(float));

            if (data == null) { return 0; }

            float value = BitConverter.ToSingle(data);

            return value;
        }

        public double ReadDouble()
        {
            byte[] data = ReadRaw(sizeof(double));

            if (data == null) { return 0; }

            double value = BitConverter.ToDouble(data);

            return value;
        }

        public byte[] ReadBytes()
        {
            // data length
            int length = ReadInt();

            // data
            byte[] data = ReadRaw(length);

            return data;
        }

        private byte[] ReadRaw(int length)
        {
            if (length <= 0) { return null; }

            byte[] data = new byte[length];

            int totalReadSize = 0;

            while (totalReadSize < length)
            {
                int readSize = Stream.Read(data, totalReadSize, data.Length - totalReadSize);

                if (readSize == 0) { break; }

                totalReadSize += readSize;
            }

            return data;
        }

        private int ReadByteRaw()
        {
            int value = Stream.ReadByte();

            return value;
        }
    }
}
