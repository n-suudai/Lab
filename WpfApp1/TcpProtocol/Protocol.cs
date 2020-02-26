

using System;
using System.Reflection;

namespace TcpProtocol
{
    public abstract class RemoteEntity
    {
        public static RemoteEntity Create(string name)
        {
            Type type = Type.GetType(name);

            if (type == null) { return null; }

            ConstructorInfo constructor = type.GetConstructor(Type.EmptyTypes);

            object instance = constructor.Invoke(null);

            RemoteEntity remoteEntity = instance as RemoteEntity;

            return remoteEntity;
        }

        public abstract bool IsQuery { get; }
        public abstract bool IsResponse { get; }

        public string Name { get { return GetType().FullName; } }

        public abstract void Serialize(Serializer serializer);

        public abstract void Deserialize(Serializer serializer);
    }

    public abstract class Query : RemoteEntity
    {
        public override sealed bool IsQuery  { get { return true; } }
        public override sealed bool IsResponse { get { return false; } }
        public abstract Response Execute();
    }

    public abstract class Response : RemoteEntity
    {
        public override sealed bool IsQuery { get { return false; } }
        public override sealed bool IsResponse { get { return true; } }
    }
}
