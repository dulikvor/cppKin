#pragma once

#include "Export.h"

namespace  cppkin {
    class CPPKIN_EXPORT ConfigTags
    {
    public:
        static const char *HOST_ADDRESS;
        static const char *PORT;
        static const char *TRANSPORT_TYPE;
        static const char *SERVICE_NAME;
        static const char *DEBUG;
        static const char *SAMPLE_COUNT;
        static const char *ENCODING_TYPE;
        static const char *BATCH_SIZE;
    private:
        ConfigTags();
    };
}
