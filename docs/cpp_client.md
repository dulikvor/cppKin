# CPP Client:

## Initialization:
Include cppkin header file - `cppkin.h`.
```c++
#include "cppkin/include/cppkin.h"
```

Before we can start we need to initialize our client, should be done once per service.

lets set the our client different policies:
```c++
cppkin::CppkinParams cppkinParams;
cppkinParams.AddParam(cppkin::ConfigTags::HOST_ADDRESS,"127.0.0.1");
cppkinParams.AddParam(cppkin::ConfigTags::PORT, 9410);
cppkinParams.AddParam(cppkin::ConfigTags::SERVICE_NAME,"serivce_name");
cppkinParams.AddParam(cppkin::ConfigTags::SAMPLE_COUNT, 10000);
```
`cppkin` different policies:

| Config Tag      | Info |
| -------------   | ------------- |
| HOST_ADDRESS    | Zipkin server address in IPV4 format XXX.XXX.XXX.XXX.  |
| PORT            | Zipkin server port value, usually 9411 for the Http collector  |
| TRANSPORT_TYPE  | Which transportaion to use Scribe/Http, Http is default. |
| SERVICE_NAME    | our service name to be displayed at Zipkin UI.  |
| DEBUG           | Will same all spans and traces, default - false.  |
| SAMPLE_COUNT    | Will sample every - n % SAMPLE_COUNT == 0 span, default value = 1000.
| BATCH_SIZE      | How many spans will be packed together when addressing the zipkin server, default value = 50. |
| ENCODING_TYPE   | Dictates which encoder will be use to encode cppkin outgoing messages - json, thrift, json is default. |

Now we can call our init function:
```c++
cppkin::Init(cppkinParams);
```

## Instrumenting:
The header will publish two **user types**:
### Trace:
```c++
namespace cppkin{
  class Trace{ ... }
}
```
### Span:
```c++
namespace cppkin{
  class Span{ ... }
}
```
### zipkin's data model key insights:
- Spans and Traces are used to instrument logical operations.
- The relation between Spans is that of parent-child.
- Those relations composed a non binary tree like structure.
- A Trace acts as the root of a span tree.

First lets create a trace:
```c++
cppkin::Trace trace("our_operation_name_we_want_to_display");
```
Spans and Traces can display time occurences called - **annotations**:
```c++
trace.AddAnnotation("some_event");
```
Lets create a child span for our trace:
```c++
auto span = trace.CreateSpan("our_span's_operation_we_want_to_display");
```
Spans and Traces can be added with key-value occurences called - **tags**:
```c++
span.AddTag("our_tag_key", "our_event");
```
Now that we are done with our span, let propegate it to zipkin:
```c++
span.Submit();
```
We can do the same with our previous trace:
```c++
trace.Submit();
```

## Stopping:
once done, we can call stop, in order to release the client resources:
```c++
cppkin::Stop();
```

# Advance capabilities:

## Span header:
The span header can be retrived in order to propegate it to a differet service.

Can be done directly:
```c++
auto spanHeader = span.GetHeader();
```
Or by the b3 string format:
```c++
typedef std::unique_ptr<const char, std::function<void(char const*)>> char_ptr;
const char* b3_header_format;
span.GetHeaderB3Format(b3_header_format);
char_ptr ptr(b3_header_format, [](char const* ptr){free(ptr);});
```

## Join:
Spans can be joined - continued in seperate services.

One option is to provide the span's header in full:
```c++
Span span();
span.Join(traceHeader.Name.c_str(), traceHeader.TraceID, traceHeader.ParentID, traceHeader.ID, traceHeader.Sampled);
```
Or by using the b3 string format:
```c++
std::string header_in_b3_format = ...
Span span();
span.Join(header_in_b3_format.c_str());
```

### Passing Trace/Span across multiple stack frames:
It is possible to provide an instance of Trace/Span across many stack frames:
```c++
static void foo()
{
    auto& trace = cppkin::PopSpan(); //our trace from before
    auto span = trace.CreateSpan("span_operation");
    span.Submit();
}
#main function
cppkin::Trace trace("trace_operation");
//Lets use the span stack container in order to reach a certain stack frame.
//We still own the trace, no transfer of ownership happened.
cppkin::PushSpan(trace);
foo();
//once done we can pop the span from the stack
cppkin::PopSpan();
trace.Submit();
```
