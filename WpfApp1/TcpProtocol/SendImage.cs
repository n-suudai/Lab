
using System.Diagnostics;

namespace TcpProtocol
{
    public class SendImageQuery : Query
    {
        public int BufferWidth { get; set; }
        public int BufferHeight { get; set; }
        public int Width { get; set; }
        public int Height { get; set; }
        public byte[] ImageBuffer { get; set; }

        public SendImageQuery()
        {
            BufferWidth = 0;
            BufferHeight = 0;
            Width = 0;
            Height = 0;
            ImageBuffer = null;
        }

        public SendImageQuery(int bufferWidth, int bufferHeight, int width, int height, byte[] imageBuffer)
        {
            BufferWidth = bufferWidth;
            BufferHeight = bufferHeight;
            Width = width;
            Height = height;
            ImageBuffer = imageBuffer;
        }

        public override void Serialize(Serializer serializer)
        {
            serializer.WriteInt(BufferWidth);
            serializer.WriteInt(BufferHeight);
            serializer.WriteInt(Width);
            serializer.WriteInt(Height);
            serializer.WriteBytes(ImageBuffer);
        }

        public override void Deserialize(Serializer serializer)
        {
            BufferWidth = serializer.ReadInt();
            BufferHeight = serializer.ReadInt();
            Width = serializer.ReadInt();
            Height = serializer.ReadInt();
            ImageBuffer = serializer.ReadBytes();
        }

        public override Response Execute()
        {
            return new SendImageResponse();
        }
    }

    public class SendImageResponse : Response
    {
        public SendImageResponse()
        {

        }

        public override void Serialize(Serializer serializer)
        {

        }

        public override void Deserialize(Serializer serializer)
        {

        }
    }
}

