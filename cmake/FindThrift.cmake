find_program(THRIFT_BINARY_DIR NAMES thrift PATHS ${PROJECT_DIR}/Third_Party/bin NO_DEFAULT_PATH)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(thrift REQUIRED_VARS THRIFT_BINARY_DIR)

if(THRIFT_FOUND)
    message(STATUS ${Green}"Found Thrift binary - ${THRIFT_BINARY_DIR}"${ColourReset})
else()
    message(WARNING ${Red}"Thrift not found"${ColourReset})
endif()
