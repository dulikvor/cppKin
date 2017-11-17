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
#include "cppkin/cppkin.h"
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
To be continue :).
