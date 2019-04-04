project(_cppkin CXX)

include_directories(.)

set(binderLib "")
if(${PYTHON_BINDING} STREQUAL "sweetPy")
    set(binderLib ${PROJECT_3RD_LOC}/lib/libsweetPy${CMAKE_DEBUG_POSTFIX}.so)
    add_definitions(-DSWEETPY)
else()
    add_definitions(-DPYBIND)
endif()

add_library(_cppkin SHARED cppkinWrapper.cpp)
target_link_libraries(_cppkin ${PROJECT_3RD_LOC}/lib/libCore${CMAKE_DEBUG_POSTFIX}.so ${binderLib}  cppkin)

ADD_CUSTOM_COMMAND(TARGET _cppkin
        POST_BUILD
        COMMAND mv ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/lib_cppkin${CMAKE_DEBUG_POSTFIX}.so ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/_cppkin.so
        )

ADD_CUSTOM_COMMAND(TARGET _cppkin
        POST_BUILD
        COMMAND cp cppkin.py ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        )
