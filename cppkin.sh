#!/bin/bash

display_help(){
    echo "############################cppkin help############################"
    echo "#--with_thrift              ## Will dictate if cppkin supports    #"
    echo "#                           ## the thrift transportation layer.   #"
    echo "#-----------------------------------------------------------------#"
    echo "#--with_tests               ## Will run cppkin tests              #"
    echo "#-----------------------------------------------------------------#"
    echo "#--with_examples            ## Will compile cppkin examples       #"
    echo "#-----------------------------------------------------------------#"
    echo "#--debug                    ## debug build                        #"
    echo "###################################################################"
}

clean_cmake_cache(){
    rm -rf CMakeFiles && rm CMakeCache.txt
}

install(){
    WITH_THRIFT=OFF
    WITH_TESTS=OFF
    WITH_EXAMPLES=OFF
    BUILD_TYPE=Release
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
            --debug)
                BUILD_TYPE=Debug
            ;;
        esac
    done
    cmake -D 3RD_PARTY_INSTALL_STEP=ON -DWITH_THRIFT=$WITH_THRIFT  -DWITH_EXAMPLES=$WITH_EXAMPLES -DCMAKE_BUILD_TYPE=$BUILD_TYPE . && make
    clean_cmake_cache
    cmake -D 3RD_PARTY_INSTALL_STEP=ON -DWITH_TESTS=$WITH_TESTS -DCMAKE_BUILD_TYPE=$BUILD_TYPE . && make
    clean_cmake_cache
    cmake -D PRE_COMPILE_STEP=ON -DWITH_THRIFT=$WITH_THRIFT -DCMAKE_BUILD_TYPE=$BUILD_TYPE . && make
    clean_cmake_cache
    cmake -D COMPILATION_STEP=ON -DWITH_THRIFT=$WITH_THRIFT -D WITH_TESTS=$WITH_TESTS -DWITH_EXAMPLES=$WITH_EXAMPLES -DCMAKE_BUILD_TYPE=$BUILD_TYPE . && make
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
        clean_cache)
            clean_cmake_cache
            if [ "$0" = "$BASH_SOURCE" ]
            then
                exit 0
            else
                return 0
            fi
        ;;
        *)
            echo "supported commands - --help, install, clean_cache"
            if [ "$0" = "$BASH_SOURCE" ]
            then
                exit 0
            else
                return 0
            fi
        ;;
    esac
done

