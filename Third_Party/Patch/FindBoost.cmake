find_path(Boost_INCLUDE_DIRS NAMES boost PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
set(Boost_LIBRARIES string)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Boost REQUIRED_VARS Boost_INCLUDE_DIRS)