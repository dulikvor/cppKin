include(ExternalProject) #In order to load the ExternalProject module and to add the ExternalProject_Add command
set_directory_properties(PROPERTIES EP_PREFIX ${PROJECT_3RD_LOC}) #Sets the prefix(dir) for all installations commands
include(InstallSpdLog)
if(WITH_THRIFT)
    include(InstallThrift)
endif()
include(InstallCURL)
include(InstallCore)
include(InstallRapidJson)
include(InstallBoost_Pack)
if(UNIX AND NOT APPLE)
    if(${PYTHON_BINDING} STREQUAL "sweetPy")
        include(InstallSweetPy)
    else()
        include(InstallPyBind)
    endif()

    if(WITH_TESTS)
        include(InstallBottle)
        include(InstallBenchMark)
    endif()
endif()

