#pragma once

#include <atomic>
#include "Core/src/Export.h"

namespace cppkin
{
    class A_EXPORT Sampler
    {
    public:
        static bool AdvanceSampler();
        static bool ShouldSample();
    private:
        static std::atomic_int sampler;
    };
}
