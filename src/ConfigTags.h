#pragma once

namespace  cppkin {
    class ConfigTags
    {
    public:
        static const char *HOST_ADDRESS;
        static const char *PORT;
        static const char *TRANSPORT_TYPE;
        static const char *SERVICE_NAME;
        static const char *DEBUG;
        static const char *SAMPLE_COUNT;
        static const char *ENCODING_TYPE;
    private:
        ConfigTags(){}
    };
}
