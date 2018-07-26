if (NOT SweetPy_FOUND AND Core_FOUND)

    ExternalProject_Add(SweetPy
            URL                 https://github.com/Dudi119/sweetPy/archive/v1.0.0.tar.gz
            CONFIGURE_COMMAND   cd <SOURCE_DIR> && cmake -DsweetPy_3RD_PARTY_DIR:STRING=<INSTALL_DIR> -DsweetPy_COMPILE_STEP=ON .
            BUILD_COMMAND       cd <SOURCE_DIR> && make
            INSTALL_COMMAND     cp <SOURCE_DIR>/bin/libsweetPy.so <INSTALL_DIR>/lib
            TEST_COMMAND        ""
            )

    ExternalProject_Add_Step(SweetPy SweetPy_Install_Headers
            COMMAND     mkdir -p <INSTALL_DIR>/include/sweetPy && sh -c "cp <SOURCE_DIR>/src/*.h <INSTALL_DIR>/include/sweetPy/"
            DEPENDEES   install
            )

    if (NOT Core_FOUND)
        add_dependencies(SweetPy Core_stub)
    endif()

    ExternalProject_Get_Property(SweetPy INSTALL_DIR)

    set (SweetPy_ROOT_DIR          ${INSTALL_DIR})
    set (SweetPy_INCLUDE_DIR       ${SweetPy_ROOT_DIR}/include)
    set (SweetPy_LIBRARY_DIR       ${SweetPy_ROOT_DIR}/lib)
    set (SweetPy_FOUND             YES)

endif ()
