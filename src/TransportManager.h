#pragma once

#include <memory>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include "Transport.h"
#include "boost/lockfree/queue.hpp"
#include "Core/src/Thread.h"
#include "Span.h"

namespace cppkin
{

    class TransportManager
    {
    public:
        static TransportManager& Instance(){
            static TransportManager instance;
            return  instance;
        }
        ~TransportManager();
        void PushSpan(std::unique_ptr<Span> span);

    private:
        TransportManager();
        void TransportWorker();

    private:
        std::unique_ptr<Transport> m_transport;
        std::unique_ptr<core::Thread> m_worker;
        static constexpr int BATCH_SIZE = 1;
        boost::lockfree::queue<Span*, boost::lockfree::fixed_sized<true>,
                boost::lockfree::capacity<BATCH_SIZE * 3>> m_spanQueue;
        std::atomic_int m_currentSpanCount;
        bool m_batchReached;
        bool m_terminate;
        std::mutex m_mut;
        std::condition_variable m_cv;
    };
}
