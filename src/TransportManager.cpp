#include "TransportManager.h"
#include <vector>
#include <chrono>
#include <functional>
#include "TransportFactory.h"
#include "ConfigParams.h"

using namespace core;

namespace cppkin
{
    TransportManager& TransportManager::Instance(){
        static TransportManager instance;
        return  instance;
    }
    TransportManager::TransportManager():m_batchReached(false), m_terminate(false) {

        m_currentSpanCount = 0;
        m_transport = TransportFactory::Instance().Create(ConfigParams::Instance().GetTransportType());
        m_batchSize = ConfigParams::Instance().GetBatchSize();
        m_spanQueue.reset(new LockFreeSpansQueue( m_batchSize * 3 ));
        m_worker = std::unique_ptr<Thread>(new Thread("TransportManager", std::bind(&TransportManager::TransportWorker, this)));
    }

    TransportManager::~TransportManager() {
        if(m_terminate == false)
            Stop();
    }

    void TransportManager::Stop() {
        m_terminate = true;
        m_worker->join();
    }

    void TransportManager::PushSpan(const std::shared_ptr<span_impl>& span) {
        span_impl* spanPtr = new span_impl(*span.get());
        while(m_spanQueue->push(spanPtr) == false);
        //fetch_add returns previous value, this is why an addition of 1 was commenced
        if(m_currentSpanCount.fetch_add(1, std::memory_order_relaxed) + 1 == m_batchSize) {
            m_currentSpanCount.fetch_sub(m_batchSize, std::memory_order_relaxed);
            m_batchReached = true;
            {
                std::unique_lock<std::mutex> lock(m_mut);
                m_cv.notify_one();
            }
        }
    }

    void TransportManager::TransportWorker()
    {
        while(!m_terminate)
        {
            {
                std::unique_lock<std::mutex> lock(m_mut);
                m_cv.wait_for(lock, std::chrono::seconds(2), [this]{ return m_batchReached; });
            }
            static std::vector<std::unique_ptr<span_impl>> retrievedSpans;
            m_spanQueue->consume_all([](span_impl* span){
                retrievedSpans.emplace_back(span);});
            if(retrievedSpans.size() > 0) {
                m_transport->Submit(retrievedSpans);
            }
            m_batchReached = false;
            retrievedSpans.clear();
        }
    }
}
