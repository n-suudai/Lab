
using System.Diagnostics;

namespace TcpProtocol
{
    public class SendImageQuery : Query
    {
        public int Width { get; set; }
        public int Height { get; set; }
        public byte[] ImageBuffer { get; set; }

        public SendImageQuery()
        {
            Width = 0;
            Height = 0;
            ImageBuffer = null;
        }

        public SendImageQuery(int width, int height, byte[] imageBuffer)
        {
            Width = width;
            Height = height;
            ImageBuffer = imageBuffer;
        }

        public override void Serialize(Serializer serializer)
        {
            serializer.WriteInt(Width);
            serializer.WriteInt(Height);
            serializer.WriteBytes(ImageBuffer);
        }

        public override void Deserialize(Serializer serializer)
        {
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

