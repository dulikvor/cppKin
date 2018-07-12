if (NOT POCO_FOUND)
    if (WIN32)
    else()
        ExternalProject_Add(POCO
                DOWNLOAD_NAME       poco-1.8.1-release.tar.gz
                URL                 https://github.com/pocoproject/poco/archive/poco-1.8.1-release.tar.gz
                CONFIGURE_COMMAND   cd <SOURCE_DIR> && cmake . -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DENABLE_ENCODINGS=OFF -DENABLE_XML=OFF -DENABLE_MONGODB=OFF -DENABLE_REDIS=OFF -DENABLE_UTIL=OFF -DENABLE_NET=OFF -DENABLE_NETSSL=OFF -DENABLE_CRYPTO=OFF -DENABLE_DATA=OFF -DENABLE_DATA_SQLITE=OFF -DENABLE_DATA_MYSQL=OFF -DENABLE_DATA_ODBC=OFF -DENABLE_ZIP=OFF -DENABLE_PAGECOMPILER=OFF -DENABLE_PAGECOMPILER_FILE2PAGE=OFF
                BUILD_COMMAND       cd <SOURCE_DIR> && make
                INSTALL_COMMAND     cd <SOURCE_DIR> && make install
                TEST_COMMAND        ""
                )
    endif()

    ExternalProject_Get_Property(POCO INSTALL_DIR)

    set (POCO_ROOT_DIR          ${INSTALL_DIR})
    set (POCO_INCLUDE_DIR       ${INSTALL_DIR}/include)
    set (POCO_FOUND             YES)

endif ()

