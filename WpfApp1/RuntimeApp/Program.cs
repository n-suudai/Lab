using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Threading.Tasks;

namespace RuntimeApp
{
    class Program
    {
        static byte FloatToByte(float x)
        {
            const byte minValue = 0;
            const byte maxValue = 255;
            return Math.Clamp((byte)(x * maxValue), minValue, maxValue);
        }

        private static bool sendComplete = true;
        private static bool exitRequest = false;
        private static TcpProtocol.Peer peer = null;

        static async void StartTcpMessaging()
        {
            string host = "127.0.0.1";
            IPAddress address = IPAddress.Parse(host);
            int port = 9999;

            // 送信完了イベント
            peer.Sended += (TcpProtocol.RemoteEntity entity) => { 
                sendComplete = true;
            };

            // 受信完了イベント
            peer.Received += (TcpProtocol.RemoteEntity entity) => { };

            await peer.Connect(address, port);

            Console.WriteLine("通信開始");

            await peer.Start();

            Console.WriteLine("通信終了");
        }

        static void MainProc()
        {
            int width = 300;
            int height = 300;
            int counter = 0;

            while (!exitRequest)
            {
                Thread.Sleep(10);

                if (!sendComplete) { continue; }
                sendComplete = false;

                byte[] buffer = new byte[width * height * 3];

                TcpProtocol.SendImageQuery query = new TcpProtocol.SendImageQuery(width, height, buffer);

                for (int y = 0; y < height; y++)
                {
                    for (int x = 0; x < width; x++)
                    {
                        int xx = (x + counter) / 100;
                        int yy = (y + counter) / 100;

                        int i = 3 * (y * width + x);
                        byte color = FloatToByte(((xx + yy) % 2 == 0) ? 0.2f : 0.8f);
                        buffer[i] = color;
                        buffer[i + 1] = color;
                        buffer[i + 2] = color;
                    }
                }

                counter += 1;

                if (counter >= int.MaxValue)
                {
                    counter = 0;
                }

                peer.Send(query);
            }
        }

        static void Main(string[] args)
        {
            peer = new TcpProtocol.Peer(SynchronizationContext.Current);

            StartTcpMessaging();

            MainProc();

            Console.ReadLine();
            exitRequest = true;
        }
    }
}
