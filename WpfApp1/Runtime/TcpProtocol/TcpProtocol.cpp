#include "TcpProtocol.h"

#include "SendImage.h"

namespace TcpProtocol
{
    std::shared_ptr<RemoteEntity> RemoteEntity::Create(const std::string& name)
    {
        if (name == "TcpProtocol.SendImageQueue")
        {
            return std::make_shared<SendImageQuery>();
        }

        if (name == "TcpProtocol.SendImageResponse")
        {
            return std::make_shared<SendImageResponse>();
        }

        return nullptr;
    }
}

