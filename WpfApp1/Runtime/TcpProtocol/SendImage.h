#pragma once


#include "TcpProtocol.h"
#include <vector>


namespace TcpProtocol
{
    class SendImageQuery final : public Query
    {
    public:
        bool Serialize(Serializer& serializer) const override;

        bool Deserialize(Serializer& serializer) override;

        const char* Name() const override;

        std::unique_ptr<Response> Execute() override;

    public:
        int buffer_width;
        int buffer_height;
        int width;
        int height;
        std::vector<unsigned char> imageBuffer;
    };


    class SendImageResponse final : public Response
    {
    public:
        bool Serialize(Serializer& serializer) const override;

        bool Deserialize(Serializer& serializer) override;

        const char* Name() const override;
    };
}