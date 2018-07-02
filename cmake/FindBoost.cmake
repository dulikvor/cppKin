find_path(BOOST_INCLUDE_DIR NAMES boost PATHS ${PROJECT_DIR}/Third_Party/include NO_DEFAULT_PATH)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(boost REQUIRED_VARS BOOST_INCLUDE_DIR)

if(BOOST_FOUND)
    message(STATUS ${Green}"Found Boost include dir - ${BOOST_INCLUDE_DIR}"${ColourReset})
else()
    message(WARNING ${Red}"Boost not found"${ColourReset})
endif()
