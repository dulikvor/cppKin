#include "Annotation.h"

using namespace std;

namespace cppkin
{
    const char* Annotation::Value::SERVER_SEND = "ss";
    const char* Annotation::Value::SERVER_RECEIVE = "sr";
    const char* Annotation::Value::CLIENT_SEND = "cs";
    const char* Annotation::Value::CLIENT_RECEIVE = "cr";
    const char* Annotation::Value::NOP = "nop";

    Annotation::EndPoint::EndPoint(const std::string &serviceName, std::string host, uint_fast16_t port)
            : ServiceName(serviceName), Host(host), Port(port)
    {
        size_t pos;
        int index = 3;
        while ((pos = host.find('.')) != std::string::npos) {
            string subAddress = host.substr(0, pos);
            host.erase(0, pos + 1);
            BinaryHost = BinaryHost | stoi(subAddress) << ( 8 * index-- );
        }
        BinaryHost = BinaryHost | ( stoi(host) << ( 8 * index ) );
    }
}

