find_path(SPDLOG_INCLUDE_DIR NAMES spdlog/spdlog.h)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(spdlog REQUIRED_VARS SPDLOG_INCLUDE_DIR)

if(SPDLOG_FOUND)
    message(STATUS "Found SpdLog include dir - ${SPDLOG_INCLUDE_DIR}")
else()
    message(WARNING "SpdLog not found")
endif()

