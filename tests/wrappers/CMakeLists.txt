project(_cppkin CXX)

cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_COLOR_MAKEFILE ON)
set (CMAKE_CXX_EXTENSIONS OFF)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Default build type 'Debug'")
    set(CMAKE_BUILD_TYPE DEBUG CACHE STRING "" FORCE )
else()
    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_DEBUG_POSTFIX d)
endif()

set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${PROJECT_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${PROJECT_DIR}/bin)
set(CMAKE_BINARY_DIR ${PROJECT_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_DIR}/bin)
set(PROJECT_3RD_LOC ${CMAKE_CURRENT_SOURCE_DIR}/../../Third_Party)

include_directories(. ../../ ${PYTHON_INCLUDE_DIRS} ${PROJECT_3RD_LOC}/include )

add_library(_cppkin SHARED cppkinWrapper.cpp)
target_link_libraries(_cppkin ${PROJECT_3RD_LOC}/lib/libCore${CMAKE_DEBUG_POSTFIX}.so ${PROJECT_3RD_LOC}/lib/libsweetPy${CMAKE_DEBUG_POSTFIX}.so cppkin)

ADD_CUSTOM_COMMAND(TARGET _cppkin
        POST_BUILD
        COMMAND mv ./bin/lib_cppkin${CMAKE_DEBUG_POSTFIX}.so ../_cppkin.so
        )
