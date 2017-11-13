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


#define INIT(params) \
    core::Enviorment::Instance().Init(); \
	cppkin::ConfigParams::Instance().Load(params);

#define CREATE_TRACE(operationName) \
    do \
    { \
        uint_fast64_t id = cppkin::Trace::Instance().GenerateTraceID(); \
        std::unique_ptr<cppkin::Span> span = cppkin::Trace::Instance().CreateSpan(operationName, id); \
        cppkin::SpanContainer::Instance().SetSpan(move(span)); \
    }while(0)

#define CREATE_SPAN(operationName, traceID, parentID)  \
    do  \
    {   \
        uint_fast64_t id = cppkin::Trace::Instance().GenerateSpanID(); \
        std::unique_ptr<cppkin::Span> span = cppkin::Trace::Instance().CreateSpan(operationName, traceID, parentID, id); \
        cppkin::SpanContainer::Instance().SetSpan(move(span)); \
    }while(0)

#define JOIN_SPAN(operationName, traceID, parentID, spanID, id) \
    do  \
    {   \
        std::unique_ptr<cppkin::Span> span = cppkin::Trace::Instance().CreateSpan(operationName, traceID, parentID, id); \
        cppkin::SpanContainer::Instance().SetSpan(move(span)); \
    }while(0)

#define SERIALIZE_TRACING_HEADER(encodingType, outputData) \
    do  \
    {   \
        cppkin::Span& span = cppkin::SpanContainer::Instance().GetSpan(); \
        cppkin::EncoderContext##encodingType context; \
        cppkin::Encoder<EncodingTypes::encodingType>::Serialize(context, span.GetHeader()); \
        outputData = move(context.ToString()); \
    }while(0)

#define DESERIALIZE_TRACING_HEADER(decodingType, data, length, outputHeader) \
    do  \
    {   \
        cppkin::EncoderContext##decodingType context(data, length); \
        outputHeader = cppkin::Encoder<EncodingTypes::decodingType>::DeSerializeSpanHeader(context); \
    }while(0)

#define SUBMIT_SPAN()  \
    do  \
    {   \
        std::unique_ptr<cppkin::Span> span = cppkin::SpanContainer::Instance().ReleaseSpan();   \
        span->SetEndTime(); \
        cppkin::TransportManager::Instance().PushSpan(move(span)); \
    }while(0)

#define TRACE_EVENT(event) \
    do  \
    {   \
        cppkin::Span& span = cppkin::SpanContainer::Instance().GetSpan();   \
        span.CreateSimpleAnnotation(event); \
    }while(0)
