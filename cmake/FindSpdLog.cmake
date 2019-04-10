find_path(SPDLOG_INCLUDE_DIR NAMES spdlog.h PATHS ${PROJECT_3RD_LOC}/include/spdlog)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(spdlog REQUIRED_VARS SPDLOG_INCLUDE_DIR)

if(SPDLOG_FOUND)
    message(STATUS "Found SpdLog include dir - ${Green}${SPDLOG_INCLUDE_DIR}${ColourReset}")
else()
    message(WARNING ${Red}"SpdLog not found"${ColourReset})
endif()

