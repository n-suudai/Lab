#include "Serializer.h"
#include <memory>


namespace TcpProtocol
{
    Serializer::Serializer(NetworkStream& stream)
        : m_stream(stream)
    {

    }

    Serializer::~Serializer()
    {

    }

    bool Serializer::WriteString(const std::string& str)
    {
        // data length
        int length = (int)str.length();
        if (!WriteInt(length)) { return false; }

        // data
        return m_stream.Write(str.data(), 0, length);
    }

    bool Serializer::WriteInt(int value)
    {
        return m_stream.Write((const char*)&value, 0, sizeof(int));
    }

    bool Serializer::WriteBuffer(const std::vector<unsigned char>& buffer)
    {
        // data length
        int length = (int)buffer.size();
        if (!WriteInt(length)) { return false; }

        // data
        if (!m_stream.Write((const char*)buffer.data(), 0, length)) { return false; }

        return true;
    }

    bool Serializer::ReadString(std::string& str)
    {
        // data length
        int length = 0;
        if (!ReadInt(length)) { return false; }

        std::vector<char> buffer((size_t)(length + 1));
        if (!m_stream.Read(buffer.data(), 0, length)) { return false; }

        buffer[length] = '\0';
        str = buffer.data();

        return true;
    }

    bool Serializer::ReadInt(int& value)
    {
        if (!m_stream.Read((char*)&value, 0, sizeof(int)))
        {
            value = 0;
            return false;
        }

        return true;
    }

    bool Serializer::ReadBuffer(std::vector<unsigned char>& buffer)
    {
        // data length
        int length = 0;
        if (!ReadInt(length)) { return false; }

        // data
        buffer.resize((size_t)length);
        if (!m_stream.Read((char*)buffer.data(), 0, length)) { return false; }

        return true;
    }
}

