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

    static void CreateTrace(const char* operationName){
        if(!cppkin::Sampler::AdvanceSampler()) {
            cppkin::SpanContainer::Instance().Reset();
            return;
        }
        uint_fast64_t id = cppkin::Trace::Instance().GenerateTraceID();
        std::unique_ptr<cppkin::Span> span = std::move(cppkin::Trace::Instance().CreateSpan(operationName, id));
        span->CreateSimpleAnnotation(operationName);
        cppkin::SpanContainer::Instance().PushSpan(std::move(span));
    }

    static void CreateSpan(const char* operationName){
        cppkin::SpanContainer& spanContainer = cppkin::SpanContainer::Instance();
        if (spanContainer.ShouldSample() == false)
            return;

        const cppkin::Span::SpanHeader& parentHeader = spanContainer.TopSpan().GetHeader();

        uint_fast64_t id = cppkin::Trace::Instance().GenerateSpanID();
        std::unique_ptr<cppkin::Span> span = cppkin::Trace::Instance().CreateSpan(operationName, parentHeader.TraceID, parentHeader.ID, id);
        span->CreateSimpleAnnotation(operationName);
        spanContainer.PushSpan(std::move(span));
    }

    static void JoinSpan(const cppkin::Span::SpanHeader& spanHeader) {
        cppkin::SpanContainer& spanContainer = cppkin::SpanContainer::Instance();
        if (spanContainer.ShouldSample() == false) //Even if we are dealing with trace, since we might be engadged in different service, the (ShouldSample) condition is retrieved from the sample context.
            return;

        std::unique_ptr<cppkin::Span> span;
        if(spanHeader.ParentIDSet)
             span = std::move(cppkin::Trace::Instance().CreateSpan(spanHeader.Name.c_str(), spanHeader.TraceID, spanHeader.ParentID, spanHeader.ID));
        else
            span = std::move(cppkin::Trace::Instance().CreateSpan(spanHeader.Name.c_str(), spanHeader.TraceID));

        spanContainer.PushSpan(std::move(span));
    }

    static std::string SerializeTracingHeader() {

        // Temporary disabled

        //    cppkin::Span* span = cppkin::SpanContainer::Instance().GetRootHeader();
        //    if(!span){
        //        return std::string();
        //    }

        //    cppkin::EncoderContext##encodingType context;
        //    cppkin::Encoder<EncodingType::encodingType>::Serialize(context, span.GetHeader());
        //    return std::move(context.ToString());

        return std::string();
    }

    static void SetTracingHeader(const char* data, size_t length) {
        if(length == 0)
            return;

        // Set parent into SpanContainer
        //cppkin::EncoderContext##decodingType context(data, length);
        // outputHeader = cppkin::Encoder<EncodingType::decodingType>::DeSerializeSpanHeader(context);
    }

    static void SubmitSpan() {
        cppkin::SpanContainer& spanContainer = cppkin::SpanContainer::Instance();
        if (spanContainer.ShouldSample() == false)
            return;

        std::unique_ptr<cppkin::Span> span = spanContainer.PopSpan();
        if(span) {
            span->SetEndTime();
            cppkin::TransportManager::Instance().PushSpan(std::move(span));
        }
    }

    static void TraceEvent(const char* eventName) {
        cppkin::SpanContainer& spanContainer = cppkin::SpanContainer::Instance();
        if (spanContainer.ShouldSample() == false)
            return;
        cppkin::Span& span = spanContainer.TopSpan();
        span.CreateSimpleAnnotation(eventName);
    }

    class SpanGuard {
    public:
        SpanGuard(const char* operationName) { CreateSpan(operationName); }
        ~SpanGuard() { SubmitSpan(); }
    };
}
