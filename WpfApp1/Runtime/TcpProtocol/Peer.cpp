#include "Peer.h"


namespace TcpProtocol
{
    Peer::Peer()
        : m_wsaData()
        , m_connectedSocket(INVALID_SOCKET)
        , m_sendQueue()
        , m_mutex()
        , m_enqueued()
        , m_sendThread()
        , m_receiveThread()
    {

    }

    Peer::~Peer()
    {
        Disconnect();
        WaitThread();
    }

    // �҂��󂯁i����j
    void Peer::Accept(const char* host, int port)
    {} // TODO : ������

    // �҂��󂯁i�������ς݁j
    void Peer::Accept()
    {} // TODO : ������

    bool Peer::Connect(const char* host, int port)
    {
        if (IsConnected())
        {
            Disconnect();
        }

        int result = ::WSAStartup(WINSOCK_VERSION, &m_wsaData);
        if (result != 0)
        {
            return false;
        }

        struct addrinfo* addrinfo = nullptr;
        struct addrinfo hints;
        ::memset(&hints, 0, sizeof(hints));

        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        char portBuffer[64];
        ::sprintf_s(portBuffer, "%d", port);

        result = ::getaddrinfo(host, portBuffer, &hints, &addrinfo);
        if (result != 0)
        {
            Disconnect();
            return false;
        }

        for (struct addrinfo* ptr = addrinfo; ptr != nullptr; ptr = addrinfo->ai_next)
        {
            m_connectedSocket = ::socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
            if (m_connectedSocket == INVALID_SOCKET)
            {
                Disconnect();
                return false;
            }

            result = ::connect(m_connectedSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (result == SOCKET_ERROR)
            {
                ::closesocket(m_connectedSocket);
                m_connectedSocket = INVALID_SOCKET;
                continue;
            }

            // connected
            break;
        }

        ::freeaddrinfo(addrinfo);

        return true;
    }

    void Peer::Send(const std::shared_ptr<RemoteEntity>& remoteEntity)
    {
        if (!IsConnected()) { return; }

        {
            std::lock_guard<std::mutex> lock(m_mutex);

            m_sendQueue.Enqueue(remoteEntity);
        }

        m_enqueued.notify_one();
    }

    // ���M�L���[���̑S�Ẵ��b�Z�[�W�����M���ꂽ��A����M���I��������
    void Peer::Terminate()
    {
        Send(nullptr);
    }

    void Peer::Start()
    {
        m_sendThread = std::thread(&Peer::SendMain, this);
        m_receiveThread = std::thread(&Peer::ReceiveMain, this);
    }

    void Peer::Listen(const char* host, int port)
    {} // TODO : ������

    void Peer::Disconnect()
    {
        if (IsConnected())
        {
            int result = ::closesocket(m_connectedSocket);
            if (result != 0)
            {
                // TODO : ERROR
            }
            m_connectedSocket = INVALID_SOCKET;

            result = ::WSACleanup();
            if (result != 0)
            {
                // TODO : ERROR
            }
        }
    }

    void Peer::SendMain()
    {
        while (IsConnected())
        {
            NetworkStream stream(m_connectedSocket);
            Serializer serializer(stream);

            // �L���[�ɒǉ������̂�҂�
            std::unique_lock<std::mutex> lock(m_mutex);
            m_enqueued.wait(lock, [this]() { return !m_sendQueue.IsEmpty(); });

            std::shared_ptr<RemoteEntity> remoteEntity = nullptr;
            if (m_sendQueue.TryDequeue(remoteEntity))
            {
                if (remoteEntity == nullptr) { break; } // nullptr ���������_�ŏI��

                if (!serializer.WriteString(remoteEntity->Name())) { break; }

                if (!remoteEntity->Serialize(serializer)) { break; }

                // TODO : ���M�����ʒm
            }
        }

        Disconnect();
    }

    void Peer::ReceiveMain()
    {
        while (IsConnected())
        {
            NetworkStream stream(m_connectedSocket);
            Serializer serializer(stream);

            std::string name = "";
            if (!serializer.ReadString(name)) { break; }

            std::shared_ptr<RemoteEntity> remoteEntity = RemoteEntity::Create(name);

            if (remoteEntity == nullptr) { break; }

            if (!remoteEntity->Deserialize(serializer)) { break; }

            // TODO : ��M�����ʒm
        }

        Terminate();
    }

    bool Peer::IsConnected() const
    {
        return m_connectedSocket != INVALID_SOCKET;
    }

    void Peer::WaitThread()
    {
        if (m_receiveThread.joinable())
        {
            m_receiveThread.join();
            m_receiveThread = std::thread();
        }

        if (m_sendThread.joinable())
        {
            m_sendThread.join();
            m_sendThread = std::thread();
        }
    }
}

