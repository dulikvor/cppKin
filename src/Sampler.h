#pragma once

#include <atomic>
#include "Core/src/Export.h"

namespace cppkin
{
    class A_EXPORT Sampler
    {
    public:
        static void AdvanceSampler();
        static bool ShouldSample();
    private:
        static std::atomic_int sampler;
#if defined(WIN32)
        static int& currentSample;
#else

#  include <features.h>
#  if __GNUC_PREREQ(4,8)
        static thread_local int currentSample;
#else
        __thread int Sampler::currentSample = 0;
#endif

#endif
    };
}
