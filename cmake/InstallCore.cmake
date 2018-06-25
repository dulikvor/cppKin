if (NOT Core_FOUND)
    ExternalProject_Add(Core
            GIT_REPOSITORY      https://github.com/Dudi119/Core
            CONFIGURE_COMMAND   cd <SOURCE_DIR> && cmake -DCORE_3RD_PARTY_DIR:STRING=<INSTALL_DIR> -DCORE_SPDLOG_SUPPORT=OFF -DCORE_COMPILE_STEP=ON .
            BUILD_COMMAND       cd <SOURCE_DIR> && make
            INSTALL_COMMAND     mkdir -p <INSTALL_DIR>/lib && cp <SOURCE_DIR>/bin/libCore.so <INSTALL_DIR>/lib
            TEST_COMMAND        ""
            )

    ExternalProject_Add_Step(Core Core_Install_Headers
            COMMAND     mkdir -p <INSTALL_DIR>/include/core && sh -c "cp <SOURCE_DIR>/src/*.h <INSTALL_DIR>/include/core/"
            DEPENDEES   install
            )

    ExternalProject_Get_Property(Core INSTALL_DIR)

    set (Core_ROOT_DIR          ${INSTALL_DIR})
    set (Core_INCLUDE_DIR       ${Core_ROOT_DIR}/include)
    set (Core_LIBRARY_DIR       ${Core_ROOT_DIR}/lib)
    set (Core_FOUND             YES)

endif ()
