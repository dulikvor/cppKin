include(ExternalProject) #In order to load the ExternalProject module and to add the ExternalProject_Add command
set_directory_properties(PROPERTIES EP_PREFIX ${PROJECT_3RD_LOC}) #Sets the prefix(dir) for all installations commands
include(InstallSpdLog)
if(WITH_THRIFT)
    include(InstallThrift)
endif()
include(InstallBoost_Pack)
include(InstallCURL)
include(InstallCore)
include(InstallPOCO)
if(UNIX AND NOT APPLE AND WITH_TESTS)
    include(InstallBottle)
    include(InstallSweetPy)
    include(InstallPyBind)
    include(InstallBenchMark)
endif()

