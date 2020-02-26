#pragma once


#include "Factory.h"
#include "Serializer.h"
#include <string>
#include <memory>


namespace TcpProtocol
{
    class RemoteEntity
    {
        static Factory<RemoteEntity> s_factory;
    public:
        static bool Initialize();

        static std::shared_ptr<RemoteEntity> Create(const std::string& name);

        virtual bool IsQuery() const = 0;

        virtual bool IsResponse() const = 0;

        virtual bool Serialize(Serializer& serializer) const = 0;

        virtual bool Deserialize(Serializer& serializer) = 0;

        virtual const char* Name() const = 0;
    };

    class Response : public RemoteEntity
    {
    public:
        bool IsQuery() const final { return false; }

        bool IsResponse() const final { return true; }
    };

    class Query : public RemoteEntity
    {
    public:
        bool IsQuery() const final { return true; }

        bool IsResponse() const final { return false; }

        virtual std::unique_ptr<Response> Execute() = 0;
    };
}
