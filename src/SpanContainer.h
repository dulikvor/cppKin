#pragma once

#include <thread>
#include <memory>
#include <forward_list>
#include "Core/src/Export.h"
#include "Span.h"

namespace cppkin
{
    class A_EXPORT SpanContainer
    {
    public:
        struct TraceInfo {
        public:
            TraceInfo():Sampled(false){}
        public:
            std::unique_ptr<Span::SpanHeader>        RootHeader;
            std::forward_list<std::unique_ptr<Span>> Spans;
            bool                                     Sampled;
        };

    public:
        static SpanContainer& Instance();
        ~SpanContainer();
        const Span& TopSpan() const;
        Span& TopSpan();
        const Span::SpanHeader& GetRootHeader() const;
        void PushSpan(std::unique_ptr<Span>&& span);
        std::unique_ptr<Span> PopSpan();
        bool ShouldSample() const;

        void Init(std::unique_ptr<TraceInfo>&& traceInfo) { m_traceInfo = std::move(traceInfo); }
        void Reset() { m_traceInfo = std::make_shared<TraceInfo>(); }
        std::shared_ptr<TraceInfo> GetTraceInfo() { return m_traceInfo; }
        void SetTraceInfo( std::shared_ptr<TraceInfo>& traceInfo ) { m_traceInfo = traceInfo; }
     private:
        SpanContainer();

    private:
        std::shared_ptr<TraceInfo> m_traceInfo;
    };
}

