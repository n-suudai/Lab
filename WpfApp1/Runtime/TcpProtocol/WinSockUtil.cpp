#include "WinSockUtil.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <sstream>


namespace WinSockUtil
{
    bool CleanUp()
    {
        int error = ::WSACleanup();
        if (error != 0)
        {
            return false;
        }

        return true;
    }

    std::string GetLastErrorText()
    {
        return GetErrorText(::WSAGetLastError());
    }

    std::string GetErrorText(int errorCode)
    {
        std::string text = "";

        switch (errorCode)
        {
        case WSAEINTR:
            text = "WSAEINTR : Interrupted function call";
            break;

        case WSAEACCES:
            text = "WSAEACCES : Permission denied";
            break;

        case WSAEFAULT:
            text = "WSAEFAULT : Bad address";
            break;

        case WSAEINVAL:
            text = "WSAEINVAL : Invalid argument";
            break;

        case WSAEMFILE:
            text = "WSAEMFILE : Too many open files";
            break;

        case WSAEWOULDBLOCK:
            text = "WSAEWOULDBLOCK : Resource temporarily unavailable";
            break;

        case WSAEINPROGRESS:
            text = "WSAEINPROGRESS : Operation now in progress";
            break;

        case WSAEALREADY:
            text = "WSAEALREADY : Operation already in progress";
            break;

        case WSAENOTSOCK:
            text = "WSAENOTSOCK : Socket operation on nonsocket";
            break;

        case WSAEDESTADDRREQ:
            text = "WSAEDESTADDRREQ : Destination address required";
            break;

        case WSAEMSGSIZE:
            text = "WSAEMSGSIZE : Message too long";
            break;

        case WSAEPROTOTYPE:
            text = "WSAEPROTOTYPE : Protocol wrong type for socket";
            break;

        case WSAENOPROTOOPT:
            text = "WSAENOPROTOOPT : Bad protocol option";
            break;

        case WSAEPROTONOSUPPORT:
            text = "WSAEPROTONOSUPPORT : Protocol not supported";
            break;

        case WSAESOCKTNOSUPPORT:
            text = "WSAESOCKTNOSUPPORT : Socket type not supported";
            break;

        case WSAEOPNOTSUPP:
            text = "WSAEOPNOTSUPP : Operation not supported";
            break;

        case WSAEPFNOSUPPORT:
            text = "WSAEPFNOSUPPORT : Protocol family not supported";
            break;

        case WSAEAFNOSUPPORT:
            text = "WSAEAFNOSUPPORT : Address family not supported by protocol family";
            break;

        case WSAEADDRINUSE:
            text = "WSAEADDRINUSE : Address already in use";
            break;

        case WSAEADDRNOTAVAIL:
            text = "WSAEADDRNOTAVAIL : Cannot assign requested address";
            break;

        case WSAENETDOWN:
            text = "WSAENETDOWN : Network is down";
            break;

        case WSAENETUNREACH:
            text = "WSAENETUNREACH : Network is unreachable";
            break;

        case WSAENETRESET:
            text = "WSAENETRESET : Network dropped connection on reset";
            break;

        case WSAECONNABORTED:
            text = "WSAECONNABORTED : Software caused connection abort";
            break;

        case WSAECONNRESET:
            text = "WSAECONNRESET : Connection reset by peer";
            break;

        case WSAENOBUFS:
            text = "WSAENOBUFS : No buffer space available";
            break;

        case WSAEISCONN:
            text = "WSAEISCONN : Socket is already connected";
            break;

        case WSAENOTCONN:
            text = "WSAENOTCONN : Socket is not connected";
            break;

        case WSAESHUTDOWN:
            text = "WSAESHUTDOWN : Cannot send after socket shutdown";
            break;

        case WSAETIMEDOUT:
            text = "WSAETIMEDOUT : Connection timed out";
            break;

        case WSAECONNREFUSED:
            text = "WSAECONNREFUSED : Connection refused";
            break;

        case WSAEHOSTDOWN:
            text = "WSAEHOSTDOWN : Host is down";
            break;

        case WSAEHOSTUNREACH:
            text = "WSAEHOSTUNREACH : No route to host";
            break;

        case WSAEPROCLIM:
            text = "WSAEPROCLIM : Too many processes";
            break;

        case WSASYSNOTREADY:
            text = "WSASYSNOTREADY : Network subsystem is unavailable";
            break;

        case WSAVERNOTSUPPORTED:
            text = "WSAVERNOTSUPPORTED : Winsock.dll version out of range";
            break;

        case WSANOTINITIALISED:
            text = "WSANOTINITIALISED : Successful WSAStartup not yet performed";
            break;

        case WSAEDISCON:
            text = "WSAEDISCON : Graceful shutdown in progress";
            break;

        case WSATYPE_NOT_FOUND:
            text = "WSATYPE_NOT_FOUND : Class type not found";
            break;

        case WSAHOST_NOT_FOUND:
            text = "WSAHOST_NOT_FOUND : Host not found";
            break;

        case WSATRY_AGAIN:
            text = "WSATRY_AGAIN : Nonauthoritative host not found";
            break;

        case WSANO_RECOVERY:
            text = "WSANO_RECOVERY : This is a nonrecoverable error";
            break;

        case WSANO_DATA:
            text = "WSANO_DATA : Valid name, no data record of requested type";
            break;

        case WSA_INVALID_HANDLE:
            text = "WSA_INVALID_HANDLE : Specified event object handle is invalid";
            break;

        case WSA_INVALID_PARAMETER:
            text = "WSA_INVALID_PARAMETER : One or more parameters are invalid";
            break;

        case WSA_IO_INCOMPLETE:
            text = "WSA_IO_INCOMPLETE : Overlapped I/O event object not in signaled state";
            break;

        case WSA_IO_PENDING:
            text = "WSA_IO_PENDING : ";
            break;

        case WSA_NOT_ENOUGH_MEMORY:
            text = "WSA_NOT_ENOUGH_MEMORY : Insufficient memory available";
            break;

        case WSA_OPERATION_ABORTED:
            text = "WSA_OPERATION_ABORTED : Overlapped operation aborted";
            break;

        case WSA_MAXIMUM_WAIT_EVENTS:
            text = "WSA_MAXIMUM_WAIT_EVENTS : ";
            break;

        case WSA_WAIT_FAILED:
            text = "WSA_WAIT_FAILED : ";
            break;

        case WSA_WAIT_TIMEOUT:
            text = "WSA_WAIT_TIMEOUT : ";
            break;

        case WSA_WAIT_IO_COMPLETION:
            text = "WSA_WAIT_IO_COMPLETION : ";
            break;

        case WSASYSCALLFAILURE:
            text = "WSASYSCALLFAILURE : System call failure";
            break;

        default:
        {
            std::stringstream ss;
            ss << errorCode << " : Unknown error";
            text = ss.str();
        }
            break;
        }

        return text;
    }
}


