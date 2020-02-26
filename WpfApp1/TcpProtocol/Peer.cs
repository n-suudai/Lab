using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TcpProtocol
{
    public sealed class Peer
    {
        public event Action<RemoteEntity> Sended;
        public event Action<RemoteEntity> Received;

        private SynchronizationContext synchronization = null;
        private ConcurrentQueue<RemoteEntity> sendQueue = new ConcurrentQueue<RemoteEntity>();
        private ManualResetEvent enqueuedWaitHandle = null;

        private TcpListener listener = null;
        private TcpClient client = null;

        public Peer(SynchronizationContext synchronization)
        {
            this.synchronization = synchronization;
            enqueuedWaitHandle = new ManualResetEvent(false);
        }

        private void Listen(IPAddress address, int port)
        {
            listener = new TcpListener(address, port);
            listener.Start();
            ServicePointManager.SetTcpKeepAlive(true, 60 * 60 * 1000, 1000);
        }

        // 待ち受け（初回）
        public async Task Accept(IPAddress address, int port)
        {
            Listen(address, port);
            await Accept();
        }

        // 待ち受け（初期化済み）
        public async Task Accept()
        {
            client = await listener.AcceptTcpClientAsync();
        }

        public async Task Connect(IPAddress address, int port)
        {
            bool failed = false;
            try
            {
                client = new TcpClient();
                await client.ConnectAsync(address, port);
            }
            catch (ArgumentNullException e)
            {
                failed = true;
                Console.WriteLine("ArgumentNullException: {0}", e);
            }
            catch (SocketException e)
            {
                failed = true;
                Console.WriteLine("SocketException: {0}", e);
            }
            catch (IOException e)
            {
                failed = true;
                Console.WriteLine("IOException: {0}", e);
            }
            finally
            {
                failed = true;
            }

            if (failed)
            {
                client = null;
            }
        }

        public void Send(RemoteEntity remoteEntity)
        {
            if (client == null) { return; }

            sendQueue.Enqueue(remoteEntity);
            enqueuedWaitHandle.Set();
        }

        // 送信キュー内の全てのメッセージが送信された後、送受信を終了させる
        public void Terminate()
        {
            Send(null);
        }

        public async Task Start()
        {
            if (client == null) { return; }

            await Task.WhenAll(
                Task.Factory.StartNew(SendMain, TaskCreationOptions.LongRunning),
                Task.Factory.StartNew(ReceiveMain, TaskCreationOptions.LongRunning));
        }

        private void SendMain()
        {
            try
            {
                Serializer serializer = new Serializer(client.GetStream());

                while (client.Connected)
                {
                    // キューに追加されるのを待つ
                    enqueuedWaitHandle.WaitOne();
                    enqueuedWaitHandle.Reset();

                    RemoteEntity remoteEntity = null;
                    if (sendQueue.TryDequeue(out remoteEntity))
                    {
                        if (remoteEntity == null) { break; } // null が来た時点で終了

                        serializer.WriteString(remoteEntity.Name);

                        remoteEntity.Serialize(serializer);

                        // post sended
                        if (synchronization != null)
                        {
                            synchronization.Post(_ => Sended?.Invoke(remoteEntity), null);
                        }
                        else
                        {
                            Sended?.Invoke(remoteEntity);
                        }
                    }
                }
            }
            catch (ArgumentNullException e)
            {
                Console.WriteLine("ArgumentNullException: {0}", e);
            }
            catch (SocketException e)
            {
                Console.WriteLine("SocketException: {0}", e);
            }
            catch(IOException e)
            {
                Console.WriteLine("IOException: {0}", e);
            }
            finally
            {
                
            }

            client.Close();
        }

        private void ReceiveMain()
        {
            try
            {
                Serializer serializer = new Serializer(client.GetStream());

                while (client.Connected)
                {
                    string entityName = serializer.ReadString();

                    RemoteEntity remoteEntity = RemoteEntity.Create(entityName);

                    if (remoteEntity == null) { break; }

                    remoteEntity.Deserialize(serializer);

                    // post received
                    if (synchronization != null)
                    {
                        synchronization.Post(_ => Received?.Invoke(remoteEntity), null);
                    }
                    else
                    {
                        Received?.Invoke(remoteEntity);
                    }
                }
            }
            catch (ArgumentNullException e)
            {
                Console.WriteLine("ArgumentNullException: {0}", e);
            }
            catch (SocketException e)
            {
                Console.WriteLine("SocketException: {0}", e);
            }
            catch (IOException e)
            {
                Console.WriteLine("IOException: {0}", e);
            }
            finally
            {
                
            }

            Terminate();
        }

        

    }
}
