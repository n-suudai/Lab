#include "TcpProtocol.h"

#include "QueryResponse/SendImage.h"

namespace TcpProtocol
{
    Factory<RemoteEntity> RemoteEntity::s_factory;

    bool RemoteEntity::Initialize()
    {
        bool result = true;

        result = result && s_factory.Register("TcpProtocol.SendImageQuery", std::make_unique<Creator<SendImageQuery, RemoteEntity>>());
        result = result && s_factory.Register("TcpProtocol.SendImageResponse", std::make_unique<Creator<SendImageResponse, RemoteEntity>>());

        return result;
    }

    std::shared_ptr<RemoteEntity> RemoteEntity::Create(const std::string& name)
    {
        return s_factory.CreateShared(name);
    }
}

