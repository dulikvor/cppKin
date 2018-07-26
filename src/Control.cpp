#include "Control.h"
#include "core/Environment.h"
#include "ConfigParams.h"
#include "TransportManager.h"

using namespace core;

namespace cppkin
{
    void Init(const GeneralParams& params){
        Environment::Instance().Init();
        ConfigParams::Instance().Load(params);
    }

    void Stop()
    {
        TransportManager::Instance().Stop();
    }
}
