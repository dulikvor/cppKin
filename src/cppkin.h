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

    static inline void CreateTrace(const char* operationName){
        if(!cppkin::Sampler::AdvanceSampler()) {
            cppkin::SpanContainer::Instance().ResetRootHeader();
            return;
        }
        uint_fast64_t id = cppkin::Trace::Instance().GenerateTraceID();
        std::unique_ptr<cppkin::Span> span = std::move(cppkin::Trace::Instance().CreateSpan(operationName, id));
        cppkin::SpanContainer::Instance().PushSpan(std::move(span));
    }

    static inline void CreateSpan(const char* operationName){

        if (!cppkin::Sampler::ShouldSample())
            return;

        cppkin::SpanContainer& spanContainer = cppkin::SpanContainer::Instance();
        const cppkin::Span::SpanHeader* parentHeader = spanContainer.GetRootHeader();
        if(!parentHeader) // No parent available. Span always has to have a parent. Skipping
            return;

        const cppkin::Span* currentSpan = spanContainer.TopSpan();
        if(currentSpan) {
            parentHeader = &currentSpan->GetHeader();
        }

        uint_fast64_t id = cppkin::Trace::Instance().GenerateSpanID();
        std::unique_ptr<cppkin::Span> span = cppkin::Trace::Instance().CreateSpan(operationName, parentHeader->TraceID, parentHeader->ID, id);
        spanContainer.PushSpan(std::move(span));
    }

    static inline void JoinSpan(const cppkin::Span::SpanHeader& spanHeader) {

        if (!cppkin::Sampler::ShouldSample())
            return;

        std::unique_ptr<cppkin::Span> span;
        if(spanHeader.ParentIDSet)
             span = std::move(cppkin::Trace::Instance().CreateSpan(spanHeader.Name.c_str(), spanHeader.TraceID, spanHeader.ParentID, spanHeader.ID));
        else
            span = std::move(cppkin::Trace::Instance().CreateSpan(spanHeader.Name.c_str(), spanHeader.TraceID));
        cppkin::SpanContainer::Instance().PushSpan(std::move(span));

    }

    static inline std::string SerializeTracingHeader() {

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

    static inline void SetTracingHeader(const char* data, size_t length) {
        if(length == 0)
            return;

        // Set parent into SpanContainer
        //cppkin::EncoderContext##decodingType context(data, length);
        // outputHeader = cppkin::Encoder<EncodingType::decodingType>::DeSerializeSpanHeader(context);
    }

    static inline void SubmitSpan() {
        std::unique_ptr<cppkin::Span> span = cppkin::SpanContainer::Instance().PopSpan();
        if(span) {
            span->SetEndTime();
            cppkin::TransportManager::Instance().PushSpan(std::move(span));
        }
    }

    static inline void TraceEvent(const char* eventName) {
        const cppkin::Span* span = cppkin::SpanContainer::Instance().TopSpan();
        if(span) {
            const_cast<cppkin::Span*>(span)->CreateSimpleAnnotation(eventName);
        }
    }

    class SpanGuard {
    public:
        SpanGuard(const char* operationName) { CreateSpan(operationName); }
        ~SpanGuard() { SubmitSpan(); }
    };
}
