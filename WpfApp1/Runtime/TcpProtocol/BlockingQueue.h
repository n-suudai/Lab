#pragma once


#include <mutex>
#include <condition_variable>
#include <deque>


namespace TcpProtocol
{
    template<typename T>
    class BlockingQueue final
    {
    public:
        inline void Enqueue(const T& value)
        {
            bool wasEmpty = true;

            {
                std::unique_lock<std::mutex> lock(m_mutex);
                wasEmpty = m_queue.empty();

                m_queue.push_back(value);
            }

            if (wasEmpty)
            {
                m_enqueued_condition.notify_one();
            }
        }

        inline bool TryDequeue(T& out, std::chrono::milliseconds waitDuration = std::chrono::milliseconds(10))
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (!m_enqueued_condition.wait_for(lock, waitDuration, [this]() { return !m_queue.empty(); }))
            {
                return false;
            }

            out = m_queue.front();
            m_queue.pop_front();
            return true;
        }

        inline bool IsEmpty()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_queue.empty();
        }

    private:
        std::mutex m_mutex;
        std::condition_variable m_enqueued_condition;
        std::deque<T> m_queue;

    };
}

