#include "Sampler.h"
#include "ConfigParams.h"

using namespace std;

namespace cppkin
{
    atomic_int Sampler::sampler(0);
    thread_local int Sampler::currentSample = 0;

    void Sampler::AdvanceSampler() {
        currentSample = atomic_fetch_add(&Sampler::sampler, 1) + 1;
    }
    bool Sampler::ShouldSample() {
        static int sampleCount = ConfigParams::Instance().GetSampleCount();
        return  currentSample % sampleCount == 0;
    }
}
