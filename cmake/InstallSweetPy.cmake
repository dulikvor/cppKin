if (NOT SweetPy_FOUND AND Core_FOUND)

    ExternalProject_Add(SweetPy
            GIT_REPOSITORY      https://github.com/Dudi119/sweetPy
            CONFIGURE_COMMAND   cd <SOURCE_DIR> && cmake -DsweetPy_3RD_PARTY_DIR:STRING=<INSTALL_DIR> -DsweetPy_COMPILE_STEP=ON -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} .
            BUILD_COMMAND       cd <SOURCE_DIR> && make
            INSTALL_COMMAND     cp <SOURCE_DIR>/bin/libsweetPy${CMAKE_DEBUG_POSTFIX}.so <INSTALL_DIR>/lib
            TEST_COMMAND        ""
            DEPENDS             Core
            )

    ExternalProject_Add_Step(SweetPy SweetPy_Install_Headers
            COMMAND     mkdir -p <INSTALL_DIR>/include/sweetPy && sh -c "cp <SOURCE_DIR>/src/*.h <INSTALL_DIR>/include/sweetPy/"
            DEPENDEES   install
            )
    ExternalProject_Add_Step(SweetPy SweetPy_Install_Core_Headers
            COMMAND     mkdir -p <INSTALL_DIR>/include/sweetPy/Core && sh -c "cp <SOURCE_DIR>/src/Core/*.h <INSTALL_DIR>/include/sweetPy/Core"
            DEPENDEES   install
            )

    ExternalProject_Add_Step(SweetPy SweetPy_Install_Types_Headers
            COMMAND     mkdir -p <INSTALL_DIR>/include/sweetPy/Types && sh -c "cp <SOURCE_DIR>/src/Types/*.h <INSTALL_DIR>/include/sweetPy/Types"
            DEPENDEES   install
            )

    if (NOT Core_FOUND)
        add_dependencies(SweetPy Core_stub)
    endif()

    set(CPPKIN_WRAPPER_DEPEND_LIST ${CPPKIN_DEPEND_LIST} SweetPy)
    ExternalProject_Get_Property(SweetPy INSTALL_DIR)

    set (SweetPy_ROOT_DIR          ${INSTALL_DIR})
    set (SweetPy_INCLUDE_DIR       ${SweetPy_ROOT_DIR}/include)
    set (SweetPy_LIBRARY_DIR       ${SweetPy_ROOT_DIR}/lib)
    set (SweetPy_FOUND             YES)

endif ()
