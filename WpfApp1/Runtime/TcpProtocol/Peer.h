#pragma once

#include <memory>
#include <functional>


#include "Serializer.h"
#include "TcpProtocol.h"
#include "BlockingQueue.h"

#include <WinSock2.h>
#include <WS2tcpip.h>


namespace TcpProtocol
{
    class Peer final
    {
    public:
        Peer();

        ~Peer();

        // �҂��󂯁i����j
        void Accept(const char* host, int port);

        // �҂��󂯁i�������ς݁j
        void Accept();

        bool Connect(const char* host, int port);

        void Send(const std::shared_ptr<RemoteEntity>& remoteEntity);

        // ���M�L���[���̑S�Ẵ��b�Z�[�W�����M���ꂽ��A����M���I��������
        void Terminate();

        void Start();

    private:
        void Listen(const char* host, int port);

        void Disconnect();

        void SendMain();

        void ReceiveMain();

        bool IsConnected() const;

        void WaitThread();

    private:
        WSADATA m_wsaData;
        SOCKET m_connectedSocket;
        BlockingQueue<std::shared_ptr<RemoteEntity>> m_sendQueue;
        std::mutex m_mutex;
        std::condition_variable m_enqueued;
        std::thread m_sendThread;
        std::thread m_receiveThread;
    };
}

