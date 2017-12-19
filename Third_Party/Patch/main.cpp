#include <string>
#include "src/cppkin.h"
#include "Core/src/Logger.h"
#include "Core/src/TraceListener.h"


using namespace std;
using namespace core;

int main()
{
    Logger::Instance().AddListener(make_shared<FileRotationListener>(TraceSeverity::Info, "./cppkinClient", 50 * 1024 * 1024, 20));
    Logger::Instance().Start(TraceSeverity::Info);

    cppkin::GeneralParams cppkinParams;
    cppkinParams.AddParam(cppkin::ConfigTags::HOST_ADDRESS, string("127.0.0.1"));
    cppkinParams.AddParam(cppkin::ConfigTags::PORT, 9410);
    cppkinParams.AddParam(cppkin::ConfigTags::SERVICE_NAME, string("Cluster_Manager"));
    cppkinParams.AddParam(cppkin::ConfigTags::DEBUG, true);
    cppkinParams.AddParam(cppkin::ConfigTags::SAMPLE_COUNT, 1);
    INIT(cppkinParams);

    CREATE_TRACE("Test");
    TRACE_EVENT("Event1");
    SUBMIT_SPAN();
    CREATE_TRACE("Test2");
    TRACE_EVENT("Event2");
    SUBMIT_SPAN();

    ::sleep(10);
    return 0;
}
