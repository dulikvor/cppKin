#!/bin/bash

display_help(){
    echo "############################cppkin help############################"
    echo "#--with_thrift              ## Will dictate if cppkin supports    #"
    echo "#                           ## the thrift transportation layer.   #"
    echo "#-----------------------------------------------------------------#"
    echo "#--with_tests               ## Will run cppkin tests              #"
    echo "#-----------------------------------------------------------------#"
    echo "#--with_examples            ## Will compile cppkin examples       #"
    echo "###################################################################"
}

clean_cmake_cache(){
    rm -rf CMakeFiles && rm CMakeCache.txt
}

install(){
    WITH_THRIFT=OFF
    WITH_TESTS=OFF
    WITH_EXAMPLES=OFF
    for argument in "${@:2}"
    do
        case $argument in
            --with_thrift)
                WITH_THRIFT=ON
            ;;
            --with_tests)
                WITH_TESTS=ON
            ;;
            --with_examples)
                WITH_EXAMPLES=ON
            ;;
        esac
    done
    cmake -D 3RD_PARTY_INSTALL_STEP=ON -DWITH_THRIFT=$WITH_THRIFT -D WITH_TESTS=$WITH_TESTS -DWITH_EXAMPLES=$WITH_EXAMPLES . && make
    clean_cmake_cache
    cmake -D PRE_COMPILE_STEP=ON -DWITH_THRIFT=$WITH_THRIFT . && make
    clean_cmake_cache
    cmake -D COMPILATION_STEP=ON -DWITH_THRIFT=$WITH_THRIFT -D WITH_TESTS=$WITH_TESTS -DWITH_EXAMPLES=$WITH_EXAMPLES . && make
    clean_cmake_cache
}

while true; do
    case $1 in 
        -h | --help)
            display_help
            if [ "$0" = "$BASH_SOURCE" ]
            then
                exit 0
            else
                return 0
            fi
        ;;
        install)
            install $@
            if [ "$0" = "$BASH_SOURCE" ]
            then
                exit 0
            else
                return 0
            fi
        ;;
        *)
            echo "supported commands - --help, install"
            if [ "$0" = "$BASH_SOURCE" ]
            then
                exit 0
            else
                return 0
            fi
        ;;
    esac
done

