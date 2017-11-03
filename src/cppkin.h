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
        uint_fast64_t id = cppkin::Trace::Instance().GenerateTraceID(); \
        std::unique_ptr<cppkin::Span> span = cppkin::Trace::Instance().CreateSpan(operationName, traceID, parentID, id); \
        cppkin::SpanContainer::Instance().SetSpan(move(span)); \
    }while(0)

#define JOIN_SPAN(operationName, traceID, parentID, spanID)

#define SUBMIT_SPAN()  \
    do  \
    {   \
        std::unique_ptr<cppkin::Span> span = cppkin::SpanContainer::Instance().ReleaseSpan();   \
        span->SetEndTime(); \
        cppkin::TransportManager::Instance().PushSpan(move(span)); \
    }while(0)

#define TRACE_CLIENT_SEND_EVENT()
#define TRACE_CLIENT_RECEIVE_EVENT()
#define TRACE_SERVER_SEND_EVENT()
#define TRACE_SERVER_RECEIVE_EVENT()

#define TRACE_EVENT(event) \
    do  \
    {   \
        cppkin::Span& span = cppkin::SpanContainer::Instance().GetSpan();   \
        span.CreateSimpleAnnotation(event); \
    }while(0)
