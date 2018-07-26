#if defined(WIN32)
#include <windows.h>
#endif
#include <string>
#include <boost/program_options.hpp>
#include "src/cppkin.h"
#include "core/Logger.h"

using namespace std;
using namespace core;
using namespace boost::program_options;

int main( int argc, const char *argv[] )
{
    options_description desc{"Options"};
        desc.add_options()
          ("help,h", "Help screen")
          ("transport", value<string>()->default_value("http"), "Transport" )
          ("encoding", value<string>()->default_value("json"), "Encoding" )
          ("host", value<string>()->default_value("127.0.0.1"), "Host" )
          ("port", value<int>()->default_value(-1), "Port")
          ("service", value<string>()->default_value("example_service"), "Service");

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    if (vm.count("help")) {
        std::cout << desc << '\n';
        return 0;
    }

    Logger::Instance().Start(TraceSeverity::Info);

    int port = 9411;
    auto transportType = cppkin::TransportType::Http;
    if (vm["transport"].as<string>() == "scribe") {
        transportType = cppkin::TransportType::Scribe;
        if (vm["port"].as<int>() == -1 ) {
            port = 9410;
        }
    }
    auto encodingType = cppkin::EncodingType::Json;
    if (vm["encoding"].as<string>() == "thrift") {
        encodingType = cppkin::EncodingType::Thrift;
        if (vm["port"].as<int>() == -1 ) {
            port = 9410;
        }
    }

    cppkin::CppkinParams cppkinParams;
    cppkinParams.AddParam(cppkin::ConfigTags::HOST_ADDRESS, vm["host"].as<string>());
    cppkinParams.AddParam(cppkin::ConfigTags::PORT, port);
    cppkinParams.AddParam(cppkin::ConfigTags::SERVICE_NAME, vm["service"].as<string>());
    cppkinParams.AddParam(cppkin::ConfigTags::DEBUG, true);
    cppkinParams.AddParam(cppkin::ConfigTags::SAMPLE_COUNT, 1);
    cppkinParams.AddParam(cppkin::ConfigTags::TRANSPORT_TYPE, cppkin::TransportType(transportType).ToString());
    cppkinParams.AddParam(cppkin::ConfigTags::ENCODING_TYPE, cppkin::EncodingType(encodingType).ToString());

    cppkin::Init(cppkinParams);

    cppkin::Trace trace("TestTrace");
    sleep(1);
    trace.AddAnnotation("TraceEvent1");

    auto span_1 = trace.CreateSpan("Span1");
    sleep(1);
    span_1.AddAnnotation("Event2");
    {
        auto span_2 = trace.CreateSpan("Span2");
        sleep(1);
        span_2.Submit();
    }
    span_1.Submit();
    sleep(1);
    trace.Submit();

#if defined(WIN32)
    ::Sleep(100);
#else
    ::sleep(2);
#endif
    Logger::Instance().Terminate();
    return 0;
}
