if (NOT BOOST_FOUND)
    ExternalProject_Add(Boost
                    DOWNLOAD_NAME       boost_1_65_1.tar.gz
                    URL                 https://dl.bintray.com/boostorg/release/1.65.1/source/boost_1_65_1.tar.gz
                    CONFIGURE_COMMAND   cd <SOURCE_DIR> && ./bootstrap.sh --prefix=<INSTALL_DIR> --exec-prefix=<INSTALL_DIR>
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     cd <SOURCE_DIR> && ./b2 install
                    TEST_COMMAND        ""
    )

ExternalProject_Get_Property(Boost INSTALL_DIR)

set (BOOST_ROOT_DIR          ${INSTALL_DIR})
set (BOOST_INCLUDE_DIR       ${INSTALL_DIR}/include)
set (BOOST_FOUND             YES)

endif ()
