using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WpfApp1
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        TcpProtocol.Peer peer = new TcpProtocol.Peer(SynchronizationContext.Current);
        bool isExitRequested = false;

        public MainWindow()
        {
            InitializeComponent();

            this.SizeToContent = SizeToContent.WidthAndHeight;

            RuntimeImage.Width = Width;
            RuntimeImage.Height = Height;

            ClearRuntimeImage();
            StartTcpMessaging(true);
        }

        private async void StartTcpMessaging(bool init)
        {
            if (init)
            {
                // 送信完了イベント
                peer.Sended += (TcpProtocol.RemoteEntity entity) => { };

                // 受信完了イベント
                peer.Received += (TcpProtocol.RemoteEntity entity) => {
                    if (entity.GetType() == typeof(TcpProtocol.SendImageQuery))
                    {
                        TcpProtocol.SendImageQuery query = (TcpProtocol.SendImageQuery)entity;
                        UpdateImage(query.ImageBuffer, query.Width, query.Height);
                    }
                };

                await peer.Accept(IPAddress.Any, 9099);
            }
            else
            {
                await peer.Accept();
            }

            Debug.WriteLine("通信開始");

            await peer.Start();

            Debug.WriteLine("通信終了");

            ClearRuntimeImage();

            if (!isExitRequested)
            {
                // 再起動
                StartTcpMessaging(false);
            }
        }

        public void OnReceiveImage(byte[] imageBuffer, int width, int height)
        {
            Dispatcher.Invoke(() =>
            {
                UpdateImage(imageBuffer, width, height);
            });
        }

        public void ClearRuntimeImage()
        {
            Dispatcher.Invoke(() =>
            {
                const int width = 10;
                const int height = 10;
                byte[] imageBuffer = new byte[width * height * 4];

                for (int y = 0; y < height; y++)
                {
                    for (int x = 0; x < width; x++)
                    {
                        int i = 4 * (y * width + x);
                        imageBuffer[i] = 0;
                        imageBuffer[i + 1] = 0;
                        imageBuffer[i + 2] = 0;
                        imageBuffer[i + 3] = 255;
                    }
                }

                UpdateImage(imageBuffer, width, height);
            });
        }

        private void UpdateImage(byte[] imageBuffer, int width, int height)
        {
            int stride = (width * PixelFormats.Pbgra32.BitsPerPixel + 7) / 8;
            BitmapSource image = BitmapSource.Create(width, height, 96, 96, PixelFormats.Pbgra32, null, imageBuffer, stride);
            ScaleTransform transform = new ScaleTransform(RuntimeImage.Width / (double)width, RuntimeImage.Height / (double)height, 0, 0);
            TransformedBitmap transformed = new TransformedBitmap(image, transform);
            RuntimeImage.Source = transformed;
        }

        private void Window_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            RuntimeImage.Width = this.Width;
            RuntimeImage.Height = this.Height;
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            isExitRequested = true;
            peer.Terminate();
        }
    }
}

