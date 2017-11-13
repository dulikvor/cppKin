#include "TransportManager.h"
#include <vector>
#include <chrono>
#include <functional>
#include "TransportFactory.h"
#include "ConfigParams.h"

using namespace core;
using namespace std;
using namespace std::chrono_literals;

namespace cppkin
{
    TransportManager::TransportManager():m_batchReached(false), m_terminate(false) {

        m_currentSpanCount = 0;
        m_transport = TransportFactory::Instance().Create(ConfigParams::Instance().GetTransportType());
        m_worker = make_unique<Thread>("TransportManager", bind(&TransportManager::TransportWorker, this));
        m_worker->Start();
    }

    TransportManager::~TransportManager() {
        m_terminate = true;
        m_worker->Join();
    }

    void TransportManager::PushSpan(unique_ptr<Span> span){
        m_spanQueue.push(span.release());
        //fetch_add returns previous value, this is why an addition of 1 was commenced
        if(atomic_fetch_add(&m_currentSpanCount, 1) + 1 == BATCH_SIZE){
            m_currentSpanCount-=BATCH_SIZE;
            m_batchReached = true;
            {
                unique_lock<mutex> lock(m_mut);
                m_cv.notify_one();
            }
        }
    }

    void TransportManager::TransportWorker()
    {
        while(!m_terminate)
        {
            {
                unique_lock<mutex> lock(m_mut);
                m_cv.wait_until(lock, chrono::system_clock::now() + 10s, [this]{return m_batchReached;});
            }
            static vector<Span*> retrievedSpans;
            m_spanQueue.consume_all([](Span* span){retrievedSpans.push_back(span);});
            if(retrievedSpans.size() > 0) {
                m_transport->Submit(retrievedSpans);
                for (Span *span : retrievedSpans) {
                    delete span;
                }
            }
            retrievedSpans.clear();
        }
    }
}
