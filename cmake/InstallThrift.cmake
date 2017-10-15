if (NOT THRIFT_FOUND)
    ExternalProject_Add(Thrift
            DOWNLOAD_NAME       thrift-0.10.0.tar.gz
            URL                 http://archive.apache.org/dist/thrift/0.10.0/thrift-0.10.0.tar.gz
            CONFIGURE_COMMAND   cd <SOURCE_DIR> && ./configure --prefix=<INSTALL_DIR> --exec-prefix=<INSTALL_DIR> --without-go --without-python
            BUILD_COMMAND       make -C <SOURCE_DIR>
            INSTALL_COMMAND     make -C <SOURCE_DIR> install
            TEST_COMMAND        ""
            )

    ExternalProject_Get_Property(Thrift INSTALL_DIR)

    set (THRIFT_ROOT_DIR          ${INSTALL_DIR})
    set (THRIFT_FOUND             YES)

endif ()
