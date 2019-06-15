#!/bin/bash
SCRIPT_DIR=$(dirname "$0")

display_help(){
    echo "                                                               "
    echo "cppkin config [options]..."
    echo "Install                       Install related options."
    echo "======="
    echo "--3rd_loc_prefix=<PREFIX>     3rd party install area prefix.   "
    echo "                                                               "
    echo "--output_dir=<DIR>            cppkin installation dir.         "
    echo "                                                               "
    echo "--debug                       debug build, otherwise - release."
    echo "                                                               "
    echo "Other options                                                  "
    echo "=============                                                  "
    echo "                                                               "
    echo "--with_tests                  Run and install cppkin tests     "
    echo "                                                               "
    echo "--with_examples               Compile cppkin examples          "
    echo "                                                               "
    echo "--python_binding=<PRODUCT>    cppkin being exported by -       "
    echo "                              pyBind|sweetPy. Default=pyBind   "
}

clean_cmake_cache(){

    declare -a commands=("rm -rf CMakeFiles"
                         "rm -rf CMakeFiles"
                         "rm CMakeCache.txt"
                         "rm *.cbp"
                         "rm cmake_install.cmake"
                         "rm compile_commands.json")

    for f in *; do
        if [[ -d $f ]]; then
            for command in "${commands[@]}"; do
                cd $f
                $command 2>/dev/null || true
                cd ..
            done
        fi
    done

    for command in "${commands[@]}"; do
        $command 2>/dev/null || true
    done
}

config(){
    WITH_TESTS=OFF
    WITH_EXAMPLES=OFF
    BUILD_TYPE=Release
    THIRD_PARTY_PREFIX=""
    OUTPUT_DIR=""
    PYTHON_BINDING="pyBind"
    for argument in "${@:2}"
    do
        case $argument in
            --with_tests)
                WITH_TESTS=ON
            ;;
            --with_examples)
                WITH_EXAMPLES=ON
            ;;
            --debug)
                BUILD_TYPE=Debug
            ;;
            --3rd_loc_prefix=*)
                THIRD_PARTY_PREFIX="${argument#*=}"
            ;;
            --output_dir=*)
                OUTPUT_DIR="${argument#*=}"
            ;;
            --python_binding=*)
                PYTHON_BINDING="${argument#*=}"
            ;;
        esac
    done
    cmake $SCRIPT_DIR -DPRE_COMPILE_STEP=ON -D3RD_PARTY_INSTALL_STEP=ON -DCOMPILATION_STEP=ON -DWITH_TESTS=$WITH_TESTS -DWITH_EXAMPLES=$WITH_EXAMPLES -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DPROJECT_3RD_LOC:STRING=$THIRD_PARTY_PREFIX -DOUTPUT_DIR:STRING=$OUTPUT_DIR -DPYTHON_BINDING:STRING=$PYTHON_BINDING
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
        config)
            config $@
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
            echo "supported commands - --help, config, clean_cache"
            if [ "$0" = "$BASH_SOURCE" ]
            then
                exit 0
            else
                return 0
            fi
        ;;
    esac
done

