#pragma once

#include "Core/src/Export.h"

namespace  cppkin {
    class A_EXPORT ConfigTags
    {
    public:
        static const char *HOST_ADDRESS;
        static const char *PORT;
        static const char *TRANSPORT_TYPE;
        static const char *SERVICE_NAME;
        static const char *DEBUG;
        static const char *SAMPLE_COUNT;
    private:
        ConfigTags();
    };
}
