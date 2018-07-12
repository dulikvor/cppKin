find_package(PythonLibs 2.7 REQUIRED)
if(${PYTHONLIBS_FOUND})
    message(STATUS ${Green}"Python version ${PYTHONLIBS_VERSION_STRING} found"${ColourReset})
    if(NOT ${PYTHONLIBS_VERSION_STRING} MATCHES "^(2.7)")
        message(FATAL_ERROR ${Red}"Python version 2.7.x is requred, found - ${PYTHONLIBS_VERSION_STRING}"${ColourReset})
    endif()
endif()

