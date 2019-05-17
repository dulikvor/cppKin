find_path(RAPIDJSON_INCLUDE_DIR NAMES rapidjson/rapidjson.h PATHS ${PROJECT_3RD_LOC}/include)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RAPIDJSON REQUIRED_VARS RAPIDJSON_INCLUDE_DIR)

if(RAPIDJSON_FOUND)
    message(STATUS "Found RAPIDJSON include dir - ${Green}${RAPIDJSON_INCLUDE_DIR}${ColourReset}")
else()
    message(WARNING ${Red}"RAPIDJSON not found"${ColourReset})
endif()
