#pragma once

#include <memory>
#include "SpanContainer.h"
#include "Span.h"
#include "Trace.h"

#define CREATE_TRACE(operationName) \
    do \
    { \
        uint_64fast_t id = cppkin::Trace::Instance().GenerateTraceID(); \
        std::shared_ptr<cppkin::Span> span = cppkin::Trace::Instance().CreateSpan(operationName, id, id); \
        cppkin::SpanContainer::Instance().SetSpan(std::move(span)); \
    }while(0)

#define CREATE_SPAN(operationName, traceID, parentID)  \
    do  \
    {   \
        std::shared_ptr<cppkin::Span> span = cppkin::Trace::Instance().CreateSpan(operationName, traceID, parentID); \
        cppkin::SpanContainer::Instance().SetSpan(std::move(span)); \
    }while(0)

#define SUBMIT_SPAN()  \
    do  \
    {   \
        cppkin::Span& span = cppkin::SpanContainer::Instance().GetSpan();   \
        span.Submit();  \
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
