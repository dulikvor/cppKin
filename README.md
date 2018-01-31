# cppkin

[![Join the chat at https://gitter.im/cppKin/Lobby](https://badges.gitter.im/cppKin/Lobby.svg)](https://gitter.im/cppKin/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge) [![Build Status](https://travis-ci.org/Dudi119/cppKin.svg?branch=master)](https://travis-ci.org/Dudi119/cppKin)

`cppkin` is an instrumentation client library for `zipkin` written in C++.
meant to be used by distributed services in order to measure latency across clusters.

## Linux Requirements

* c++11.
* Cmake version > 3.8.
* OpenSSL-dev.

## Windows Requirements

* Runtime support and tool chain of Visual Studio 2013.
* Cmake version > 3.8.
* Flex and Bison (e.g. the WinFlexBison package)


## Getting Started

cppkin uses the following 3rd party dependencies -
1) boost.
2) spdlog.
3) thrift.
4) curl.

### Linux Install

In order to start run the following script:
```
. INSTALL.bash
```
The script will:
1) Retrieve all 3rd party dependecies, compiling them and installing them locally (under cppkin directory structure).
2) Compile cppkin into shared object.

`cppkin` shared object will be available under the bin dir.

### Windows Install

In order to start run the following script:
```
INSTALL.bat
```
The script will:
1) Retrieve all 3rd party dependecies, compiling them and installing them locally (under cppkin directory structure).
2) Compile cppkin into shared object.
3) Will create msbuild project structure for all the different components.

`cppkin` dll will be available under the bin dir.

### Interface

In order to use cppkin include cppkin interface file - `cppkin.h`.
```c++
#include "cppKin/src/cppkin.h"
```
The file publish the following capabilities:
1) Initialization.
2) Create Trace.
3) Create/Join Span.
4) Trace simple events.
5) Serialize/Deserialize span header.

All capabilities are provided in Macro method style (semicolon is in need), for example:
```c++
CREATE_TRACE("Resource_Offer");
```
### Initialization
In order to initialize `cppkin` (the first step you would like to take) two operations are in need:
1) Composing `cppkin` configuration.
2) Providing the config object to the `Init` routine.

#### Configuration
`cppkin` supports the following policies:

| Config Tag      | Info |
| -------------   | ------------- |
| HOST_ADDRESS    | Zipkin server address in IPV4 format XXX.XXX.XXX.XXX provided as c++ string.  |
| PORT            | Zipkin server port value, usually 9410 for Scribe collector  |
| TRANSPORT_TYPE  | Which transportaion to use, as of now only Scribe is supported and the tag is not in use. |
| SERVICE_NAME    | The traced service name which will be displayed at Zipkin UI, provided as c++ string.  |
| DEBUG           | Will mark all sampled spans as debug spans.  |
| SAMPLE_COUNT    | Will sample every - n % SAMPLE_COUNT == 0 span.

A full example:
```c++
cppkin::CppkinParams cppkinParams;
cppkinParams.AddParam(cppkin::ConfigTags::HOST_ADDRESS, string("127.0.0.1"));
cppkinParams.AddParam(cppkin::ConfigTags::PORT, 9410);
cppkinParams.AddParam(cppkin::ConfigTags::SERVICE_NAME, string("Index_Builder"));
cppkinParams.AddParam(cppkin::ConfigTags::DEBUG, false);
cppkinParams.AddParam(cppkin::ConfigTags::SAMPLE_COUNT, 1000);
cppkin::Init(cppkinParams);
```

### Storage
Each new span (acting as the new context), is referenced by using the current thread TLS, all operation are acted upon the current context. 

by creating a new span in the context of the current thread, it will be elected as the new context.

#### Tracing
In order to trace use the `cppkin::CreateTrace` command:
```c++
cppkin::CreateTrace("Scheduling tasks");
```
`cppkin::CreateTrace` takes a single argument - the `"Operation name"` which needs to be provided as `const char*`  argument type.

### Child Span
In order to create a child span use the `cppkin::CreateSpan` command:
```c++
cppkin::CreateSpan("Processing Task");
```
`cppkin::CreateSpan` takes the following arguments:
* Operation name - `const char*` type.

<!--- * The current trace id - retrieved from the previous span header. 
 * Parent span id - retrieved from the previous span header. ---> 


### Trace simple events
In order to trace an event use the `cppkin::TraceEvent` command:
```c++
cppkin::TraceEvent("Trace an event");
```
`cppkin::TraceEvent` takes a single argument - the `"Event value"` which needs to be provided as `const char*`  argument type.

### Transportation
`cppkin` contains a specified `transportation` layer, providing the following capabilities:
* Serializing the received data.
* Transporting it to a specific zipkin collector out stream.


### OutStream Communication
Outstream communication is done by using a single simple command - `SUBMIT_SPAN'.
```c++
cppkin::CreateSpan();
```
once called the current span in `context` will be out streamed to the designated collector. the `context` will be cleared after the completion of the command.

Collector type and collector address are set by using `cppkin` configuration during the `init` step.
