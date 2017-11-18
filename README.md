# cppkin

`cppkin` is an instrumentation client library for `zipkin` written in C++.
meant to be used by distributed services in order to measure latency across clusters.

## Prerequisites
Platform - cppkin was built on top of Ubuntu and uses posix, no windows version is currently supported.

Language - Uses c++14.

Build system - cppkin uses Cmake version > 3.8.

## Getting Started

cppkin uses the following 3rd party dependencies -
1) boost.
2) spdlog.
3) thrift.

In order to start run the following script:
```
. INSTALL.bash
```
The script will:
1) Retrieve all 3rd party dependecies, compiling them and installing them locally (under cppkin directory structure).
2) Compile cppkin into shared object.

The shared object will be available under the bin dir.

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

A full example:
```c++
cppkin::GeneralParams cppkinParams;
cppkinParams.AddParam(cppkin::ConfigTags::HOST_ADDRESS, string("127.0.0.1"));
cppkinParams.AddParam(cppkin::ConfigTags::PORT, 9410);
cppkinParams.AddParam(cppkin::ConfigTags::SERVICE_NAME, string("Index_Builder"));
INIT(cppkinParams);
```
