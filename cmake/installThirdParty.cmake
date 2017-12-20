include(ExternalProject) #In order to load the ExternalProject module and to add the ExternalProject_Add command
set (EXTERNAL_PROJECT_DIR ${CMAKE_BINARY_DIR}/Third_Party CACHE PATH "External projects location") #Sets a variable with the installation location of all third party
set_directory_properties(PROPERTIES EP_PREFIX ${EXTERNAL_PROJECT_DIR}) #Sets the prefix(dir) for all installations commands
include(InstallSpdLog)
include(InstallThrift)
include(InstallBoost)
include(InstallCURL)
if(UNIX AND NOT APPLE)
    include(InstallBenchMark)
endif()

