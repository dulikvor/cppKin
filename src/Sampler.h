#pragma once

#include <atomic>

namespace cppkin
{
    class Sampler
    {
    public:
        static bool AdvanceSampler();
        static bool ShouldSample();
    private:
        static std::atomic_int sampler;
    };
}
