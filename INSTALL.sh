#!/bin/bash

workingDir=$PWD
if ! echo $BOOST_ROOT | grep -q "/Third_Party/include"; then
    export "BOOST_ROOT=$BOOST_ROOT:$workingDir/Third_Party/include/"
fi
cmake -D 3RD_PARTY_INSTALL_STEP=True -D PRE_COMPILE_STEP=False -D COMPILATION_STEP=False -D TESTS_SUPPORT=ON . && make
cmake -D COMPILATION_STEP=False -D PRE_COMPILE_STEP=True -D 3RD_PARTY_INSTALL_STEP=False . && make
cmake -D COMPILATION_STEP=True -D PRE_COMPILE_STEP=False -D 3RD_PARTY_INSTALL_STEP=False -D TESTS_SUPPORT=ON . && make
