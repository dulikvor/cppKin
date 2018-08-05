# cppkin

[![Join the chat at https://gitter.im/cppKin/Lobby](https://badges.gitter.im/cppKin/Lobby.svg)](https://gitter.im/cppKin/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge) [![Build Status](https://travis-ci.org/Dudi119/cppKin.svg?branch=master)](https://travis-ci.org/Dudi119/cppKin) [![Build status](https://ci.appveyor.com/api/projects/status/riaj54pn4h08xy40?svg=true)](https://ci.appveyor.com/project/Dudi119/cppKin)

`cppkin` is an instrumentation client library for `zipkin` written in C++.
meant to be used by distributed services in order to measure latency across clusters.

## Linux Requirements
* c++11.
* Cmake version > 3.8.
* OpenSSL-dev.
If testing is requested - python 2.7

## Windows Requirements
* Runtime support and tool chain of Visual Studio 2013.
* Cmake version > 3.8.
* Flex and Bison (e.g. the WinFlexBison package)


## Getting Started
cppkin uses the following 3rd party dependencies -
1) boost.
2) spdlog.
3) thrift - only if thrift is requested.
4) curl.
5) Core.
6) sweetPy - only if testing is requested.
7) bottle - only if testing is requested.

### Linux Install
In order to inspect cppKin available installation parameters, use the help command:
```
. cppkin.bash --help
```

In order to start the installation use the install command:
```
. cppkin.bash install #add arguments of choice
```
The script will:
1) Retrieve all 3rd party dependecies, compiling them and installing them locally (under cppkin directory structure).
2) Compile cppkin into shared object.
3) Will build tests and examples if requested.

`cppkin` shared object will be available under the bin dir.

### Windows Install
In order to inspect cppKin available installation parameters, use the help command:
```
cppkin.bat --help
```

In order to start the installation use the install command:
```
cppkin.bat install #add arguments of choice
```
The script will:
1) Retrieve all 3rd party dependecies, compiling them and installing them locally (under cppkin directory structure).
2) Compile cppkin into dll.
3) Will build all examples if requested.

`cppkin` dll will be available under the bin dir.

### Interface
In order to use cppkin include cppkin interface file - `cppkin.h`.
```c++
#include "cppKin/src/cppkin.h"
```
cppkin supports an object oriented style of coding.
the header will publish two user types:
- init - Initializing the cppKin client library.
- A stop function - deallocating cppKin client library internal resources.
- A Trace type.
- A Span type.
- Pushing a reference for a specific Span/Trace.
- Popping a referenced Span/Trace.

Trace type capabilities:
- Create a Trace.
- Creating a child span.
- Adding simple annotations.
- Submitting a trace.

Span type capabilities:
- Creating a child span.
- Join a span.
- Adding simple annotations.
- Submitting a trace.

### Initialization
In order to initialize `cppkin` (the first step you would like to take) two operations are in need:
1) Composing `cppkin` configuration.
2) Providing the config object to the `Init` routine.

#### Configuration
`cppkin` supports the following policies:

| Config Tag      | Info |
| -------------   | ------------- |
| HOST_ADDRESS    | Zipkin server address in IPV4 format XXX.XXX.XXX.XXX.  |
| PORT            | Zipkin server port value, usually 9410 for Scribe collector  |
| TRANSPORT_TYPE  | Which transportaion to use Scribe/Http/Stub, Stub is default. |
| SERVICE_NAME    | The traced service name which will be displayed at Zipkin UI.  |
| DEBUG           | Will mark all sampled spans as debug spans.  |
| SAMPLE_COUNT    | Will sample every - n % SAMPLE_COUNT == 0 span, default value = 1000.
| BATCH_SIZE      | How many spans will be packed together when addressing the zipkin server, default value = 50. |
| ENCODING_TYPE   | Dictates which encoder will be use to encode cppkin outgoing messages - json, thrift, json is default. |


A full example:
```c++
cppkin::CppkinParams cppkinParams;
cppkinParams.AddParam(cppkin::ConfigTags::HOST_ADDRESS,"127.0.0.1");
cppkinParams.AddParam(cppkin::ConfigTags::PORT, 9410);
cppkinParams.AddParam(cppkin::ConfigTags::SERVICE_NAME,"Index_Builder");
cppkinParams.AddParam(cppkin::ConfigTags::DEBUG, false);
cppkinParams.AddParam(cppkin::ConfigTags::SAMPLE_COUNT, 1000);
cppkinParams.AddParam(cppkin::ConfigTags::TRANSPORT_TYPE, cppkin::TransportType(cppkin::TransportType::Http).ToString());
cppkinParams.AddParam(cppkin::ConfigTags::ENCODING_TYPE, cppkin::EncodingType(cppkin::EncodingType::Json).ToString());
cppkin::Init(cppkinParams);
```

### Stop
In order to deallocate the client library internal resources in correct fashion, cppKin provides a Stop function.
```c++
cppkin::Stop();
```
Should come at a completion point with in your flow, when usage of the client is done.

##
### Tracing
In order to create a trace, initalize a new trace instance:
```c++
cppkin::Trace trace("Something");
```
`Trace constructor` takes a single argument - the `"Operation name"`.

### Child Span
In order to create a child span use the `CreateSpan` method:
```c++
trace.CreateSpan("Span1");
```
The same interface is also supported with the span type:
```c++
auto span = trace.CreateSpan("Span1");
span.CreateSpan("Span2");
```
`CreateSpan` takes a single argument - the `"Operation name"`.

### Trace simple events
In order to trace an event use the `AddAnnotation` method:
```c++
trace.AddAnnotation("TraceEvent1");
span.AddAnnotation("TraceEvent2");
```
`AddAnnotation` takes a single argument - the `"Event value"`.

### Transportation
`cppkin` contains a specified `transportation` layer, providing the following capabilities:
* Serializing the received data.
* Transporting it to a specific zipkin collector out stream.

Supported transportation methods:

| Transport      | Encoding |
| -------------   | ------------- |
| Thrift    | Thrift  |
| Http      | Json  |
| Stub (will send nothing)| |

### OutStream Communication
Outstream communication is done by using the `Submit` method.
```c++
trace.Submit();
span.Submit();
```
once called the current Trace/Span will be out streamed at some point to designated collector.

### Span Header
Every Trace/Span instances contains a span header.
the header is used to contain the most important data depicting the current Trace/Span, including:
- Trace id.
- Span id (A trace is sharing its trace id with the span id).
- Parent Span id.
- Sampled - is the Trace/Span is sampled or not.

The header is used to keep track of our current trace context across different processes, usually it will be the header which will be serialized and provided via, for example - an RPC to the designated process.

in order to fetch the current Trace/Span header, use the following example:
```c++
auto traceHeader = trace.GetHeader();
auto spanHeader = span.GetHeader();
```
### Join a Trace/Span
In order to join an existing Trace/Span use the `Join` method and a received Trace/Span header:
```c++
Span span();
span.Join(traceHeader.Name.c_str(), traceHeader.TraceID, traceHeader.ParentID, traceHeader.ID, traceHeader.Sampled);
```

Span will always be used at the receiving side (Continuing an existing trace or span).
### Passing Trace/Span across multiple stack frames:
It is possible to provide an instance of Trace/Span across many stack frames (no injection is in need).
use the Push/Pop command, in order the provide a specific span.
```c++
static void foo()
{
    cppkin::Span& span_1 = *cppkin::PopSpan();
    auto span_2 = span_1.CreateSpan("Span2");
    portable_sleep(10);
    span_2.Submit();
}
#main function
    auto span_1 = trace.CreateSpan("Span1");
    span_1.AddAnnotation("Span1Event");
    //Lets use the span container in order to reach a certain stack frame.
    //We still own the span, no transfer of ownership was commenced.
    cppkin::PushSpan(span_1);
    foo();
    span_1.Submit();
```
Its important to remember that the ownership of that span remain at the user hands, the span is only referenced by the Push command.

