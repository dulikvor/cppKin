#pragma once

#include <memory>
#include "Core/src/Enviorment.h"
#include "ConfigParams.h"
#include "ConfigTags.h"
#include "GeneralParams.h"
#include "SpanContainer.h"
#include "Span.h"
#include "Trace.h"
#include "TransportManager.h"
#include "EncodingContext.h"
#include "Encoder.h"
#include "Sampler.h"


#define INIT(params) \
    core::Enviorment::Instance().Init(); \
	cppkin::ConfigParams::Instance().Load(params);

#define CREATE_TRACE(operationName) \
    do \
    { \
        cppkin::Sampler::AdvanceSampler(); \
        if(cppkin::Sampler::ShouldSample()){ \
            uint_fast64_t id = cppkin::Trace::Instance().GenerateTraceID(); \
            std::unique_ptr<cppkin::Span> span = cppkin::Trace::Instance().CreateSpan(operationName, id); \
            cppkin::SpanContainer::Instance().SetSpan(span.release()); \
        } \
    }while(0)

#define CREATE_SPAN(operationName, traceID, parentID)  \
    do  \
    {   \
        cppkin::Sampler::AdvanceSampler(); \
        if(cppkin::Sampler::ShouldSample()){ \
            uint_fast64_t id = cppkin::Trace::Instance().GenerateSpanID(); \
            std::unique_ptr<cppkin::Span> span = cppkin::Trace::Instance().CreateSpan(operationName, traceID, parentID, id); \
            cppkin::SpanContainer::Instance().SetSpan(span.release()); \
        } \
    }while(0)

#define JOIN_SPAN(spanHeader) \
    do  \
    {   \
        cppkin::Sampler::AdvanceSampler(); \
        if(cppkin::Sampler::ShouldSample()){ \
            std::unique_ptr<cppkin::Span> span; \
            if(spanHeader.ParentIDSet)  \
                 span = std::move(cppkin::Trace::Instance().CreateSpan(spanHeader.Name.c_str(), spanHeader.TraceID, spanHeader.ParentID, spanHeader.ID)); \
            else \
                span = std::move(cppkin::Trace::Instance().CreateSpan(spanHeader.Name.c_str(), spanHeader.TraceID)); \
            cppkin::SpanContainer::Instance().SetSpan(span.release()); \
        } \
    }while(0)

#define SERIALIZE_TRACING_HEADER(encodingType, outputData) \
    do  \
    {   \
        if(cppkin::Sampler::ShouldSample()){ \
            cppkin::Span& span = cppkin::SpanContainer::Instance().GetSpan(); \
            cppkin::EncoderContext##encodingType context; \
            cppkin::Encoder<EncodingTypes::encodingType>::Serialize(context, span.GetHeader()); \
            outputData = std::move(context.ToString()); \
        } \
        else \
            outputData = std::string(); \
    }while(0)

#define DESERIALIZE_TRACING_HEADER(decodingType, data, length, outputHeader) \
    do  \
    {   \
        if(length > 0){ \
            cppkin::EncoderContext##decodingType context(data, length); \
            outputHeader = cppkin::Encoder<EncodingTypes::decodingType>::DeSerializeSpanHeader(context); \
        } \
    }while(0)

#define SUBMIT_SPAN()  \
    do  \
    {   \
        if(cppkin::Sampler::ShouldSample()){ \
            std::unique_ptr<cppkin::Span> span = cppkin::SpanContainer::Instance().ReleaseSpan();   \
            span->SetEndTime(); \
            cppkin::TransportManager::Instance().PushSpan(std::move(span)); \
        } \
    }while(0)

#define TRACE_EVENT(event) \
    do  \
    {   \
        if(cppkin::Sampler::ShouldSample()){ \
            cppkin::Span& span = cppkin::SpanContainer::Instance().GetSpan();   \
            span.CreateSimpleAnnotation(event); \
        } \
    }while(0)
