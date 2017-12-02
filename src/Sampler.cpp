#include "Sampler.h"
#include "ConfigParams.h"

using namespace std;

namespace cppkin
{
#if defined(WIN32)
	atomic_int Sampler::sampler{ 0 };
	_declspec(thread) int Sampler::currentSample = 0;
#else
	atomic_int Sampler::sampler(0);
    thread_local int Sampler::currentSample = 0;
#endif

	void Sampler::AdvanceSampler() {
        currentSample = atomic_fetch_add(&sampler, 1) + 1;
    }
    bool Sampler::ShouldSample() {
        static int sampleCount = ConfigParams::Instance().GetSampleCount();
        return  currentSample % sampleCount == 0;
    }
}
