#pragma once


#include "NetworkStream.h"
#include <string>
#include <vector>


namespace TcpProtocol
{
    class Serializer final
    {
    public:
        Serializer(NetworkStream& stream);

        ~Serializer();

        bool WriteString(const std::string& str);

        bool WriteInt(int value);

        bool WriteBuffer(const std::vector<unsigned char>& buffer);

        bool ReadString(std::string& str);

        bool ReadInt(int& value);

        bool ReadBuffer(std::vector<unsigned char>& buffer);

    private:
        NetworkStream& m_stream;
    };
}

