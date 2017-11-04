find_program(THRIFT_BINARY_DIR NAMES thrift ${PATH_PROJECT_DIR}/Third_Party/bin NO_SYSTEM_ENVIRONMENT_PATH)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(thrift REQUIRED_VARS THRIFT_BINARY_DIR)

if(THRIFT_FOUND)
    message(STATUS "Found Thrift binary - ${THRIFT_BINARY_DIR}")
else()
    message(WARNING "Thrift not found")
endif()
