#pragma once

#include <memory>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include "Transport.h"
#include "boost/lockfree/queue.hpp"
#include "core/Thread.h"
#include "span_impl.h"

namespace cppkin
{

    class TransportManager
    {
    public:
        static TransportManager& Instance();
        ~TransportManager();
        void PushSpan(const std::shared_ptr<span_impl>& span);
        void Stop();

    private:
        TransportManager();
        void TransportWorker();

    private:
        typedef boost::lockfree::queue<span_impl*, boost::lockfree::fixed_sized<true>> LockFreeSpansQueue;
        std::unique_ptr<Transport> m_transport;
        std::unique_ptr<core::Thread> m_worker;
        int m_batchSize;
        std::unique_ptr<LockFreeSpansQueue> m_spanQueue;
        std::atomic_int m_currentSpanCount;
        bool m_batchReached;
        bool m_terminate;
        std::mutex m_mut;
        std::condition_variable m_cv;
    };
}
