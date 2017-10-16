find_path(BOOST_INCLUDE_DIR NAMES boost)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(boost REQUIRED_VARS BOOST_INCLUDE_DIR)

if(BOOST_FOUND)
    message(STATUS "Found Boost include dir - ${BOOST_INCLUDE_DIR}")
else()
    message(WARNING "Boost not found")
endif()
