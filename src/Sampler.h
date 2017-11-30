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
        static thread_local int currentSample;
    };
}
