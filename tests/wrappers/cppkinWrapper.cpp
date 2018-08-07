#include <Python.h>
#include <memory>
#include <string>
#include "sweetPy/CPythonModule.h"
#include "sweetPy/CPythonClass.h"
#include "sweetPy/CPythonGlobalFunction.h"
#include "sweetPy/CPythonGlobalVariable.h"
#include "sweetPy/InitModule.h"
#include "sweetPy/CPythonEnum.h"
#include "sweetPy/Exception.h"
#include "src/Trace.h"
#include "src/Span.h"
#include "src/cppkin.h"
#include "src/ConfigTags.h"
#include "src/Annotation.h"

INIT_MODULE(_cppkin, "cppkin library wrapper")
{
    sweetPy::CPythonClass<cppkin::Span> span(module, "Span", "cppkin span");
    span.AddConstructor<>();
    span.AddMethod("createSpan", "creating a child span", &cppkin::Span::CreateSpan);
    span.AddMethod("submit", "the trace will be submitted to the collector", &cppkin::Span::Submit);

    sweetPy::CPythonClass<cppkin::Trace> trace(module, "Trace", "cppkin trace");
    trace.AddConstructor<const char *>();
    trace.AddMethod("createSpan", "creating a child span", &cppkin::Trace::CreateSpan);
    trace.AddMethod("submit", "the trace will be submitted to the collector", &cppkin::Trace::Submit);

    sweetPy::CPythonClass<cppkin::CppkinParams> params(module, "CppkinParams", "a general key value storage for cppkin params");
    params.AddMethod("add_int", "will add an int typed param", &cppkin::CppkinParams::AddParam<int>);
    params.AddMethod("add_str", "will add a str typed param", &cppkin::CppkinParams::AddParam<std::string>);
    params.AddMethod("add_bool", "will add a bool typed param", &cppkin::CppkinParams::AddParam<bool>);

    sweetPy::CPythonGlobalVariable(module, "HOST_ADDRESS", cppkin::ConfigTags::HOST_ADDRESS);
    sweetPy::CPythonGlobalVariable(module, "PORT", cppkin::ConfigTags::PORT);
    sweetPy::CPythonGlobalVariable(module, "SERVICE_NAME", cppkin::ConfigTags::SERVICE_NAME);
    sweetPy::CPythonGlobalVariable(module, "DEBUG", cppkin::ConfigTags::DEBUG);
    sweetPy::CPythonGlobalVariable(module, "TRANSPORT_TYPE", cppkin::ConfigTags::TRANSPORT_TYPE);
    sweetPy::CPythonGlobalVariable(module, "SAMPLE_COUNT", cppkin::ConfigTags::SAMPLE_COUNT);

    sweetPy::CPythonGlobalVariable(module, "SERVER_RECEIVE", cppkin::Annotation::Value::SERVER_RECEIVE);
    sweetPy::CPythonGlobalVariable(module, "SERVER_SEND", cppkin::Annotation::Value::SERVER_SEND);

    sweetPy::CPythonGlobalFunction(module, "init", "initializes cppkin", &cppkin::Init);
    sweetPy::CPythonGlobalFunction(module, "stop", "deallocate cppkin resources", &cppkin::Stop);
}
