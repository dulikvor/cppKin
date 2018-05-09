#include "Trace.h"
#include "Sampler.h"
#include "span_impl.h"

namespace  cppkin
{
    Trace::Trace(const char *operationName, const char* value):
        Span(operationName, span_impl::GenerateID(), Sampler::AdvanceSampler())
    {
        AddAnnotation(value, m_span->GetTimeStamp());
    }
}
