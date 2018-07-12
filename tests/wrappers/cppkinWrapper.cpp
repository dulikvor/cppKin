#include <memory>
#include <string>
#include <Python.h>
#include "sweetPy/CPythonModule.h"
#include "sweetPy/CPythonClass.h"
#include "sweetPy/CPythonGlobalFunction.h"
#include "sweetPy/CPythonGlobalVariable.h"
#include "sweetPy/InitModule.h"
#include "sweetPy/CPythonEnum.h"
#include "sweetPy/Exception.h"
#include "core/Logger.h"
#include "core/Source.h"
#include "core/Logger.h"
#include "src/Trace.h"
#include "src/Span.h"
#include "src/Init.h"
#include "src/ConfigTags.h"

INIT_MODULE(_cppkin, "cppkin library wrapper")
{
    sweetPy::CPythonClass<core::Logger> logger(module, "Logger", "logger type");
    logger.AddStaticMethod("instance", "logger singelton access function", &core::Logger::Instance);
    logger.AddMethod("trace", "logger trace function", &core::Logger::Trace<>);
    logger.AddMethod("start", "Initiates the logger, commencing all accamulated listeners to underline layer.", &core::Logger::Start);

    sweetPy::CPythonClass<core::Source> source(module, "Source", "source type");
    source.AddMember("file", &core::Source::file, "source code file name");
    source.AddMember("function", &core::Source::function, "current function name");
    source.AddMember("line", &core::Source::line, "source code line");

    sweetPy::CPythonEnum severity(module, "TraceSeverity", "Trace severity levels");
    severity.AddEnumValue("Verbose", core::TraceSeverity::Verbose, "Verbose severity level");
    severity.AddEnumValue("Info", core::TraceSeverity::Info, "Info severity level");
    severity.AddEnumValue("Fatal", core::TraceSeverity::Fatal, "Fatal severity level");

    sweetPy::CPythonClass<cppkin::Span> span(module, "Span", "cppkin span");
    span.AddConstructor<>();
    span.AddMethod("createSpan", "creating a child span", &cppkin::Span::CreateSpan);
    span.AddMethod("submit", "the trace will be submitted to the collector", &cppkin::Span::Submit);

    sweetPy::CPythonClass<cppkin::Trace> trace(module, "Trace", "cppkin trace");
    trace.AddConstructor<const char *>();
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

    sweetPy::CPythonGlobalFunction(module, "init", "initializes cppkin", &cppkin::Init);
}
