#include "NetworkStream.h"

namespace TcpProtocol
{
    NetworkStream::NetworkStream(SOCKET& socket)
        : m_socket(socket)
    {

    }

    NetworkStream::~NetworkStream()
    {

    }

    bool NetworkStream::Write(const char* buffer, int offset, int count)
    {
        const char* buff = nullptr;
        int result = -1;
        int length = 0;
        while (length < count)
        {
            buff = &buffer[offset + length];

            int result = ::send(m_socket, buff, count - length, 0);

            if (result > 0)
            {
                length += result;
            }
            else
            {
                break; // ERROR
            }
        }

        return length == count;
    }

    bool NetworkStream::Read(char* buffer, int offset, int count)
    {
        char* buff = nullptr;
        int result = -1;
        int length = 0;

        while (length < count)
        {
            buff = &buffer[offset + length];

            result = ::recv(m_socket, buffer, count, 0);

            if (result > 0)
            {
                length += result;
            }
            else
            {
                break; // ERROR
            }
        }

        return length == count;
    }
}

