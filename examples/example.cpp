#if defined(WIN32)
#include <windows.h>
#endif
#include <string>
#include <boost/program_options.hpp>
#include "src/cppkin.h"

using namespace std;
using namespace core;
using namespace boost::program_options;

static void portable_sleep(int duration)
{
#if defined(WIN32)
    ::Sleep(duration);
#else
    ::sleep(duration);
#endif
}

static void foo()
{
    cppkin::Span& span_1 = *cppkin::PopSpan();
    auto span_2 = span_1.CreateSpan("Span2");
    portable_sleep(10);
    span_2.Submit();
}

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
    cppkinParams.AddParam(cppkin::ConfigTags::SAMPLE_COUNT, 1);
    cppkinParams.AddParam(cppkin::ConfigTags::TRANSPORT_TYPE, cppkin::TransportType(transportType).ToString());
    cppkinParams.AddParam(cppkin::ConfigTags::ENCODING_TYPE, cppkin::EncodingType(encodingType).ToString());

    cppkin::Init(cppkinParams);

    cppkin::Trace trace("TestTrace");
    portable_sleep(10);
    trace.AddAnnotation("TraceEvent");

    auto span_1 = trace.CreateSpan("Span1");
    portable_sleep(10);
    span_1.AddAnnotation("Span1Event");
    //Lets use the span container in order to reach a certain stack frame.
    cppkin::PushSpan(span_1);
    foo();

    span_1.Submit();
    portable_sleep(10);
    trace.Submit();

#if defined(WIN32)
    portable_sleep(100);
#else
    portable_sleep(2);
#endif
    cppkin::Stop();
    return 0;
}
