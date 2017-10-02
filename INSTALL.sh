#!/bin/bash
workingDir=$PWD
export "LIBRARY_PATH=$LIBRARY_PATH:$workingDir/Third_Party/lib"
export "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$workingDir/Third_Party/lib"
export "PATH=$PATH:$workingDir/Third_Party/bin"
export "CPATH=$CPATH:$workingDir/Third_Party/include/" && cmake -D 3RD_PARTY_INSTALL_STEP=True . && make && cmake -D COMPILATION_STEP=True . && make
