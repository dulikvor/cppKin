if (NOT PYBIND_FOUND)
    ExternalProject_Add(PyBind
            DOWNLOAD_NAME       v2.2.4.tar.gz
            URL                 https://github.com/pybind/pybind11/archive/v2.2.4.tar.gz
            CONFIGURE_COMMAND   ""
            BUILD_COMMAND       ""
            INSTALL_COMMAND     ""
            TEST_COMMAND        ""
            )

    ExternalProject_Add_Step(PyBind PyBind_Install_Headers
            COMMAND     mkdir -p <INSTALL_DIR>/include && cp -r <SOURCE_DIR>/include/pybind11/ <INSTALL_DIR>/include/
            DEPENDEES   install
            )

    set(CPPKIN_WRAPPER_DEPEND_LIST ${CPPKIN_WRAPPER_DEPEND_LIST} PyBind)
    ExternalProject_Get_Property(PyBind INSTALL_DIR)

    set (PYBIND_ROOT_DIR          ${INSTALL_DIR})
    set (PYBIND_INCLUDE_DIR       ${PYBIND_ROOT_DIR}/include)
    set (PYBIND_FOUND             YES)
endif ()
