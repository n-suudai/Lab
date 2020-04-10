#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "TcpProtocol/Peer.h"
#include "TcpProtocol/SendImage.h"


#pragma comment(lib, "ws2_32.lib")


template<typename T>
T Clamp(T value, T minValue, T maxValue)
{
    if (value < minValue) { return minValue; }

    if (value > maxValue) { return maxValue; }

    return value;
}

char FloatToByte(float x)
{
    constexpr unsigned char minValue = 0;
    constexpr unsigned char maxValue = 255;
    return Clamp((unsigned char)(x * maxValue), minValue, maxValue);
}

int main()
{
    if (!TcpProtocol::RemoteEntity::Initialize()) { return -1; }

    TcpProtocol::Peer peer;

    if (!peer.Connect("127.0.0.1", 9999)) { return -1; }

    peer.Start();

    int width = 300;
    int height = 300;
    int counter = 0;

    bool exitRequest = false;

    while (!exitRequest)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        std::shared_ptr<TcpProtocol::SendImageQuery> query = std::make_shared<TcpProtocol::SendImageQuery>();
        query->width = width;
        query->height = height;
        query->imageBuffer.resize(((size_t)width * (size_t)height * 3));

        for (size_t y = 0; y < height; y++)
        {
            for (size_t x = 0; x < width; x++)
            {
                size_t xx = (x + counter) / 100;
                size_t yy = (y + counter) / 100;

                size_t i = 3 * (y * (size_t)width + x);
                unsigned char color = FloatToByte(((xx + yy) % 2 == 0) ? 0.2f : 0.8f);
                query->imageBuffer[i] = color;
                query->imageBuffer[i + 1] = color;
                query->imageBuffer[i + 2] = color;
            }
        }

        counter += 1;

        if (counter >= std::numeric_limits<int>::max())
        {
            counter = 0;
        }

        peer.Send(query);
    }

    return 0;
}



