#include "Sampler.h"
#include "ConfigParams.h"

using namespace std;

namespace cppkin
{
#if defined(WIN32)
    atomic_int Sampler::sampler{0} ;
    _declspec(thread) int tlsCurrentSample = 0;
    int& Sampler::currentSample = tlsCurrentSample;
#else
    atomic_int Sampler::sampler{0};

#  if __GNUC_PREREQ(4,8)
    thread_local int Sampler::currentSample = 0;
#else
    __thread int Sampler::currentSample = 0;
#endif

#endif

    bool Sampler::AdvanceSampler() {
        static int sampleCount = ConfigParams::Instance().GetSampleCount();
        currentSample = atomic_fetch_add(&sampler, 1) + 1;
        return currentSample % sampleCount == 0;
    }
}
