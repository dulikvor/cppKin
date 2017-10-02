if (NOT SPDLOG_FOUND)

    ExternalProject_Add(SpdLog
            DOWNLOAD_NAME       spdlog-0.13.0.tar
            URL                 https://github.com/gabime/spdlog/archive/v0.13.0.tar.gz
            CONFIGURE_COMMAND   ""
            BUILD_COMMAND       ""
            INSTALL_COMMAND     ""
            TEST_COMMAND        ""
            )

    ExternalProject_Add_Step(SpdLog SpdLog_Install_Headers
            COMMAND     mkdir <INSTALL_DIR>/include && cp -r <SOURCE_DIR>/include/spdlog <INSTALL_DIR>/include/spdlog
            DEPENDEES   install
            )

    ExternalProject_Get_Property(SpdLog INSTALL_DIR)

    set (SPDLOG_ROOT_DIR          ${INSTALL_DIR})
    set (SPDLOG_INCLUDE_DIR       ${SPDLOG_ROOT_DIR}/include)
    set (SPDLOG_FOUND             YES)

    #    mark_as_advanced(GLOG_LIBRARY GLOG_INCLUDE_PATH)

endif ()
