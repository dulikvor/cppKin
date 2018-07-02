find_path(BOTTLE_INCLUDE_DIR NAMES bottle.py PATHS ${PROJECT_DIR}/tests NO_DEFAULT_PATH)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Bottle REQUIRED_VARS BOTTLE_INCLUDE_DIR)

if(Bottle_FOUND)
    message(STATUS ${Green}"Found Bottle include dir - ${BOTTLE_INCLUDE_DIR}"${ColourReset})
else()
    message(WARNING ${Red}"Bottle not found"${ColourReset})
endif()
