#include "Trace.h"

Trace& Trace::Instance(){
    static Trace instance;
    return instance;
}
