#pragma once

#include <atomic>

namespace cppkin
{
    class Sampler
    {
    public:
        static void AdvanceSampler();
        static bool ShouldSample();
    private:
        static std::atomic_int sampler;
#if defined(WIN32)
        static _declspec(thread) int currentSample;
#else
        static thread_local int currentSample;
#endif
    };
}
