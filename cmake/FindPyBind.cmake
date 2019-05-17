find_path(PyBind_INCLUDE_DIR NAMES pybind11/pybind11.h PATHS ${PROJECT_3RD_LOC}/include)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PyBind REQUIRED_VARS PyBind_INCLUDE_DIR)

if(PyBind_FOUND)
    message(STATUS "Found PyBind include dir - ${Green}${PyBind_INCLUDE_DIR}${ColourReset}")
else()
    message(WARNING ${Red}"PyBind not found${ColourReset}")
endif()
