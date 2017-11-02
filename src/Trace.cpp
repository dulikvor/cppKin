#include "Trace.h"
#include <random>
#include "Span.h"

using namespace std;
namespace  cppkin
{
    Trace& Trace::Instance(){
        static Trace instance;
        return instance;
    }

    unique_ptr<Span> Trace::CreateSpan(const char *operationName, uint_fast64_t traceID, uint_fast64_t parentID ) {
        return unique_ptr<Span>(new Span(operationName, traceID, parentID, GenerateSpanID()));
    }

    uint_fast64_t Trace::GenerateSpanID(){
        static mt19937_64 twister;
        static random_device device;
        uint64_t seed = (static_cast<uint64_t>(device()) << 32) | device();
        twister.seed(seed);
        return twister();
    }

    uint_fast64_t Trace::GenerateTraceID(){
        static mt19937_64 twister;
        static random_device device;
        uint64_t seed = (static_cast<uint64_t>(device()) << 32) | device();
        twister.seed(seed);
        return twister();
    }
}
