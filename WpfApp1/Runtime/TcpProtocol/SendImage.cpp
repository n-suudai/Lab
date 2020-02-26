
#include "SendImage.h"


namespace TcpProtocol
{
    // Query
    bool SendImageQuery::Serialize(Serializer& serializer) const
    {
        if (!serializer.WriteInt(width)) { return false; }
        if (!serializer.WriteInt(height)) { return false; }
        if (!serializer.WriteBuffer(imageBuffer)) { return false; }
        return true;
    }

    bool SendImageQuery::Deserialize(Serializer& serializer)
    {
        if (!serializer.ReadInt(width)) { return false; }
        if (!serializer.ReadInt(height)) { return false; }
        if (!serializer.ReadBuffer(imageBuffer)) { return false; }
        return true;
    }

    const char* SendImageQuery::Name() const
    {
        return "TcpProtocol.SendImageQuery";
    }

    std::unique_ptr<Response> SendImageQuery::Execute()
    {
        return std::make_unique<SendImageResponse>();
    }


    // Response
    bool SendImageResponse::Serialize(Serializer&) const
    {
        return true;
    }

    bool SendImageResponse::Deserialize(Serializer&)
    {
        return true;
    }

    const char* SendImageResponse::Name() const
    {
        return "TcpProtocol.SendImageResponse";
    }
}

