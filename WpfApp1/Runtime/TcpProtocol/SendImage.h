#pragma once


#include "TcpProtocol.h"
#include <vector>


namespace TcpProtocol
{
    class SendImageQuery final : public Query
    {
    public:
        bool Serialize(Serializer& serializer) const;

        bool Deserialize(Serializer& serializer);

        const char* Name() const;

        std::unique_ptr<Response> Execute();

    public:
        int width;
        int height;
        std::vector<unsigned char> imageBuffer;
    };


    class SendImageResponse final : public Response
    {
    public:
        bool Serialize(Serializer& serializer) const;

        bool Deserialize(Serializer& serializer);

        const char* Name() const;
    };
}