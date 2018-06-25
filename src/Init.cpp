#include "Init.h"
#include "core/Environment.h"
#include "ConfigParams.h"

using namespace core;

namespace cppkin
{
    void Init(const GeneralParams& params){
        Environment::Instance().Init();
        ConfigParams::Instance().Load(params);
    }
}
