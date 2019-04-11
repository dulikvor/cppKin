# cppkin

[![Join the chat at https://gitter.im/cppKin/Lobby](https://badges.gitter.im/cppKin/Lobby.svg)](https://gitter.im/cppKin/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge) [![Build Status](https://travis-ci.org/Dudi119/cppKin.svg?branch=master)](https://travis-ci.org/Dudi119/cppKin) [![Build status](https://ci.appveyor.com/api/projects/status/riaj54pn4h08xy40?svg=true)](https://ci.appveyor.com/project/Dudi119/cppKin)

`cppkin` is an instrumentation client library for `zipkin` available in **C++** and **Python**.

## Linux Requirements
* c++11.
* Cmake version > 3.8.

## Windows Requirements
* VC++ 12.0 and above.
* Cmake version > 3.8.

## Python
* pybind11.
* Python 2.7 and 3.X are supported.

## Getting Started
cppkin is dependent upon -

| Package                 | Required   |  version  |
| ----------------------- | -----------| --------- |
| boost                   | mandatory  | >= 1.65.1 |
| curl                    | mandatory  | >= 7.54.1 |
| rapidjson               | mandatory  | >= 1.1.0  |
| core                    | mandatory  | >= 1.1    |
| pybind11                | python or tests  | >= 2.2.4 |
| bottle                  | tests  |    >= 0.12.13 |
| google benchmark        | tests |     >= 1.3     |
| thrift                  | only for scribe transport | >= 0.10 |

### Linux Install
Use the help command to review the different configuration arguments:
```
. cppkin.bash --help
```

Build cppkin by using the shell script:
```
. cppkin.bash config #add arguments of choice
make
make install
```

### Windows Install
Use the help command to review the different configuration arguments:
```
cppkin.bat --help
```

Build cppkin by using the batch script:
```
cppkin.bat install #add arguments of choice
```

