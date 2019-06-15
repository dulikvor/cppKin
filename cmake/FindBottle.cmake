find_path(BOTTLE_INCLUDE_DIR NAMES bottle.py PATHS ${PROJECT_SOURCE_DIR}/tests)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Bottle REQUIRED_VARS BOTTLE_INCLUDE_DIR)

if(Bottle_FOUND)
    message(STATUS "Found Bottle include dir - ${Green}${BOTTLE_INCLUDE_DIR}${ColourReset}")
else()
    message(WARNING ${Red}"Bottle not found"${ColourReset})
endif()
