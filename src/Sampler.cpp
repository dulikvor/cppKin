#include "Sampler.h"
#include "ConfigParams.h"

using namespace std;

namespace cppkin
{
    atomic_int Sampler::sampler{0} ;

#if defined(WIN32)
   _declspec(thread) int t_currentSample = 0;
#else

#  if __GNUC_PREREQ(4,8)
    thread_local int t_currentSample = 0;
#else
    __thread int t_currentSample = 0;
#endif

#endif

    bool Sampler::AdvanceSampler() {
        t_currentSample = sampler.fetch_add(1, std::memory_order_relaxed) + 1;
        return Sampler::ShouldSample();
    }

    bool  Sampler::ShouldSample() {
       static int sampleCount = ConfigParams::Instance().GetSampleCount();
       return t_currentSample % sampleCount == 0;
    }

}
