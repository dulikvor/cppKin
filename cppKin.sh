#!/bin/bash

display_help(){
    echo "############################cppkin help############################"
    echo "#--with_scribe              ## Will dictate if cppkin supports    #"
    echo "#                           ## providing data to scribe collector.#"
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
    WITH_SCRIBE=OFF
    WITH_TESTS=OFF
    WITH_EXAMPLES=OFF
    for argument in "${@:2}"
    do
        case $argument in
            --with_scribe)
                WITH_SCRIBE=ON
            ;;
            --with_tests)
                WITH_TESTS=ON
            ;;
            --with_examples)
                WITH_EXAMPLES=ON
            ;;
        esac
    done
    cmake -D 3RD_PARTY_INSTALL_STEP=ON -DWITH_SCRIBE=$WITH_SCRIBE -D TESTS_SUPPORT=$WITH_TESTS . && make
    clean_cmake_cache
    cmake -D PRE_COMPILE_STEP=ON -DWITH_SCRIBE=$WITH_SCRIBE . && make
    clean_cmake_cache
    cmake -D COMPILATION_STEP=ON -D -DWITH_SCRIBE=$WITH_SCRIBE -D TESTS_SUPPORT=$WITH_TESTS . && make
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
        --install)
            install $@
            if [ "$0" = "$BASH_SOURCE" ]
            then
                exit 0
            else
                return 0
            fi
        ;;
        *)
            echo "supported commands - --help, --install"
            if [ "$0" = "$BASH_SOURCE" ]
            then
                exit 0
            else
                return 0
            fi
        ;;
    esac
done

