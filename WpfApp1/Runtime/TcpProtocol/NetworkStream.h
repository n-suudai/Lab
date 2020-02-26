#pragma once


#include <WinSock2.h>


namespace TcpProtocol
{
    class NetworkStream final
    {
    public:
        NetworkStream(::SOCKET& socket);

        ~NetworkStream();

        bool Write(const char* buffer, int offset, int count);

        bool Read(char* buffer, int offset, int count);

    private:
        ::SOCKET& m_socket;
    };
}

