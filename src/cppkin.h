#pragma once

#include <memory>
#include "Core/src/GeneralParams.h"
#include "Init.h"
#include "ConfigParams.h"
#include "ConfigTags.h"
#include "SpanContainer.h"
#include "Span.h"
#include "Trace.h"
#include "TransportManager.h"
#include "EncodingContext.h"
#include "EncodingTypes.h"
#include "Encoder.h"
#include "Sampler.h"

namespace cppkin {
    typedef core::GeneralParams CppkinParams;
}

class CppKin {
public:
    static void Init(const cppkin::CppkinParams& params) { cppkin::Init(params); }
    static void CreateTrace(const char* operationName);
    static void CreateSpan(const char* operationName);
    static void JoinSpan(const cppkin::Span::SpanHeader& spanHeader);
    static void TraceEvent(const char* eventName);
    static void SubmitSpan();
    static std::string SerializeTracingHeader();
    static void SetTracingHeader(char* data, size_t length);

    class SpanGuard {
    public:
        SpanGuard(const char* operationName) { CppKin::CreateSpan(operationName); }
        ~SpanGuard() { CppKin::SubmitSpan(); }
    };
};

inline void CppKin::CreateTrace(const char* operationName){
    if(!cppkin::Sampler::AdvanceSampler())
        return;
    uint_fast64_t id = cppkin::Trace::Instance().GenerateTraceID();
    std::unique_ptr<cppkin::Span> span = std::move(cppkin::Trace::Instance().CreateSpan(operationName, id));
    cppkin::SpanContainer::Instance().SetSpan(span.release());
}

inline void CppKin::CreateSpan(const char* operationName){
    cppkin::Span* currentSpan = cppkin::SpanContainer::Instance().GetSpan();
    if(!currentSpan) // No parent available. Span always has to have a parent. Skipping
        return;

    const cppkin::Span::SpanHeader& currentSpanHeader = currentSpan->GetHeader();
    uint_fast64_t id = cppkin::Trace::Instance().GenerateSpanID();
    std::unique_ptr<cppkin::Span> span = cppkin::Trace::Instance().CreateSpan(operationName, currentSpanHeader.TraceID, currentSpanHeader.ID, id);
    cppkin::SpanContainer::Instance().SetSpan(span.release());
}

inline void CppKin::JoinSpan(const cppkin::Span::SpanHeader& spanHeader) {

    if(cppkin::Sampler::AdvanceSampler()){
        std::unique_ptr<cppkin::Span> span;
        if(spanHeader.ParentIDSet)
             span = std::move(cppkin::Trace::Instance().CreateSpan(spanHeader.Name.c_str(), spanHeader.TraceID, spanHeader.ParentID, spanHeader.ID));
        else
            span = std::move(cppkin::Trace::Instance().CreateSpan(spanHeader.Name.c_str(), spanHeader.TraceID));
        cppkin::SpanContainer::Instance().SetSpan(span.release());
    }
}

static std::string SerializeTracingHeader() {
    cppkin::Span* span = cppkin::SpanContainer::Instance().GetSpan();
    if(!span){
        return std::string();
    }

    // Temporary disabled
//    cppkin::EncoderContext##encodingType context;
//    cppkin::Encoder<EncodingType::encodingType>::Serialize(context, span.GetHeader());
//    return std::move(context.ToString());

    return std::string();
}

static void SetTracingHeader(char* data, size_t length) {
    if(length == 0)
        return;

    // Set parent into SpanContainer
    //cppkin::EncoderContext##decodingType context(data, length);
    // outputHeader = cppkin::Encoder<EncodingType::decodingType>::DeSerializeSpanHeader(context);
}

inline void CppKin::SubmitSpan() {
    cppkin::SpanContainer& spanContainer = cppkin::SpanContainer::Instance();
    if(spanContainer.GetSpan()) { // Always release existing span. Do not check for ShouldSample. It was already checked on creation
        std::unique_ptr<cppkin::Span> span = spanContainer.ReleaseSpan();
        span->SetEndTime();
        cppkin::TransportManager::Instance().PushSpan(std::move(span));
    }
}

inline void CppKin::TraceEvent(const char* eventName) {
    cppkin::Span* span = cppkin::SpanContainer::Instance().GetSpan();
    if(span) {
        span->CreateSimpleAnnotation(eventName);
    }
}
