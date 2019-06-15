include(ExternalProject) #In order to load the ExternalProject module and to add the ExternalProject_Add command
set_directory_properties(PROPERTIES EP_PREFIX ${PROJECT_3RD_LOC}) #Sets the prefix(dir) for all installations commands
include(InstallSpdLog)
include(InstallCURL)
include(InstallCore)
include(InstallRapidJson)
include(InstallBoost_Pack)
if(WITH_PYTHON)
    if(${PYTHON_BINDING} STREQUAL "sweetPy")
        include(InstallSweetPy)
    else()
        include(InstallPyBind)
    endif()
    if(WITH_TESTS)
        include(InstallBottle)
    endif()
endif()

if(UNIX AND NOT APPLE AND WITH_TESTS)
	include(InstallBenchMark)
endif()

