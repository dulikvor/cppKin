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
        static thread_local int currentSample;
#endif
    };
}
