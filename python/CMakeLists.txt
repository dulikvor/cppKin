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
set_target_properties(_cppkin PROPERTIES PUBLIC_HEADER "cppkin.py")
set_target_properties(_cppkin PROPERTIES PREFIX "")
set_target_properties(_cppkin PROPERTIES DEBUG_POSTFIX "")

ADD_CUSTOM_COMMAND(TARGET _cppkin
        POST_BUILD
        COMMAND cp cppkin.py ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        )


if(WIN32)
    install(TARGETS _cppkin
            PUBLIC_HEADER DESTINATION ${OUTPUT_DIR}/python
            RUNTIME DESTINATION ${OUTPUT_DIR}/python
            )
else()
    install(TARGETS _cppkin
            PUBLIC_HEADER DESTINATION ${OUTPUT_DIR}/python
            LIBRARY DESTINATION ${OUTPUT_DIR}/python
            )
endif()

