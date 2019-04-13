#include <memory>
#include <string>
#if defined SWEETPY
#include "sweetPy/CPythonModule.h"
#include "sweetPy/CPythonClass.h"
#include "sweetPy/CPythonGlobalFunction.h"
#include "sweetPy/CPythonGlobalVariable.h"
#include "sweetPy/InitModule.h"
#include "sweetPy/CPythonEnum.h"
#include "sweetPy/Core/Exception.h"
#elif defined PYBIND
#include "pybind11/pybind11.h"
#endif
#include "src/Trace.h"
#include "src/Span.h"
#include "src/cppkin.h"
#include "src/ConfigTags.h"
#include "src/Annotation.h"

#if defined SWEETPY
INIT_MODULE(_cppkin, "cppkin library wrapper")
{
    sweetPy::CPythonClass<cppkin::Span> span(module, "Span", "cppkin span");
    span.AddConstructor<>();
    span.AddMethod("create_span", "creating a child span", &cppkin::Span::CreateSpan);
    span.AddMethod("submit", "the trace will be submitted to the collector", &cppkin::Span::Submit);

    sweetPy::CPythonClass<cppkin::Trace> trace(module, "Trace", "cppkin trace");
    trace.AddConstructor<const char *>();
    trace.AddMethod("create_span", "creating a child span", &cppkin::Trace::CreateSpan);
    trace.AddMethod("submit", "the trace will be submitted to the collector", &cppkin::Trace::Submit);

    sweetPy::CPythonClass<cppkin::CppkinParams> params(module, "CppkinParams", "a general key value storage for cppkin params");
    params.AddMethod("add_int", "will add an int typed param", &cppkin::CppkinParams::AddParam<const int&>);
    params.AddMethod("add_str", "will add a str typed param", &cppkin::CppkinParams::AddParam<const std::string&>);
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
#elif defined PYBIND
    PYBIND11_MODULE(_cppkin, module)
    {
        pybind11::class_<cppkin::Span>(module, "Span")
            .def(pybind11::init<>())
            .def("create_span", &cppkin::Span::CreateSpan)
            .def("submit", &cppkin::Span::Submit, pybind11::arg("value") = cppkin::Annotation::Value::SERVER_SEND)
            .def("add_annotation", static_cast<void(cppkin::Span::*)(const char*)>(&cppkin::Span::AddAnnotation))
            .def("add_tag", static_cast<void(cppkin::Span::*)(const char*, const char*)>(&cppkin::Span::AddTag))
            .def("get_header_b3_format", [](const cppkin::Span& self){
                const char* header_b3_format;
                self.GetHeaderB3Format(header_b3_format);
                std::unique_ptr<const char, std::function<void(const char*)>> guard(
                        header_b3_format,
                        [](const char* ptr){free(const_cast<char*>(ptr));}
                    );
                return std::string(header_b3_format);
            })
            .def("join", static_cast<void(cppkin::Span::*)(const char*)>(&cppkin::Span::Join));
        
        pybind11::class_<cppkin::Trace>(module, "Trace")
            .def(pybind11::init<const char*>())
            .def("create_span", &cppkin::Trace::CreateSpan)
            .def("submit", &cppkin::Trace::Submit, pybind11::arg("value") = cppkin::Annotation::Value::SERVER_SEND)
            .def("add_annotation", static_cast<void(cppkin::Trace::*)(const char*)>(&cppkin::Trace::AddAnnotation))
            .def("add_tag", static_cast<void(cppkin::Trace::*)(const char*, const char*)>(&cppkin::Trace::AddTag))
            .def("get_header_b3_format", [](const cppkin::Trace& self){
                const char* header_b3_format;
                self.GetHeaderB3Format(header_b3_format);
                std::unique_ptr<const char, std::function<void(const char*)>> guard(
                        header_b3_format,
                        [](const char* ptr){free(const_cast<char*>(ptr));}
                );
                return std::string(header_b3_format);
            })
            .def("join", static_cast<void(cppkin::Trace::*)(const char*)>(&cppkin::Trace::Join));
        
        pybind11::class_<cppkin::CppkinParams>(module, "CppkinParams")
            .def(pybind11::init<>())
            .def("add_int", &cppkin::CppkinParams::AddParam<const int&>)
            .def("add_str", &cppkin::CppkinParams::AddParam<const std::string&>)
            .def("add_bool", &cppkin::CppkinParams::AddParam<const bool&>);
        
        module.attr("HOST_ADDRESS") = cppkin::ConfigTags::HOST_ADDRESS;
        module.attr("PORT") = cppkin::ConfigTags::PORT;
        module.attr("SERVICE_NAME") = cppkin::ConfigTags::SERVICE_NAME;
        module.attr("DEBUG") = cppkin::ConfigTags::DEBUG;
        module.attr("TRANSPORT_TYPE") = cppkin::ConfigTags::TRANSPORT_TYPE;
        module.attr("SAMPLE_COUNT") = cppkin::ConfigTags::SAMPLE_COUNT;
        
        module.attr("SERVER_RECEIVE") = cppkin::Annotation::Value::SERVER_RECEIVE;
        module.attr("SERVER_SEND") = cppkin::Annotation::Value::SERVER_SEND;
        module.attr("NOP") = cppkin::Annotation::Value::NOP;
        
        module.def("init", &cppkin::Init, "initializes cppkin");
        module.def("stop", &cppkin::Stop, "deallocate cppkin resources");
        module.def("push_span", &cppkin::PushSpan, "pushes current span to the span container");
        module.def("top_span", &cppkin::TopSpan, pybind11::return_value_policy::reference, "retrieves current top span from the span container");
        module.def("pop_span", &cppkin::PopSpan, "pop the current top span at the span container");
        module.def("is_container_empty", &cppkin::IsContainerEmpty, "inquiries if the span container is empty");
        module.def("cast_trace_to_span", [](cppkin::Trace& trace)->cppkin::Span&{
            return static_cast<cppkin::Span&>(trace);},
            pybind11::return_value_policy::reference, "casting from trace to span");
    
        module.def("add_tag", [](const std::string& key, const std::string& value){
                    auto& span = cppkin::TopSpan();
                    span.AddTag(key.c_str(), value.c_str());},
                   "adding a new binary annotation for current span");
        module.def("add_annotation", [](const std::string& value){
                       auto& span = cppkin::TopSpan();
                       span.AddAnnotation(value.c_str());},
                   "adding a new annotation for current span");
    }
#endif
