if (NOT SPDLOG_FOUND)

    ExternalProject_Add(SpdLog
            DOWNLOAD_NAME       spdlog-0.13.0.tar
            URL                 https://github.com/gabime/spdlog/archive/v0.13.0.tar.gz
			URL_MD5             a0903d50ae1d58b484d9fc1a6ddf02f4
            CONFIGURE_COMMAND   ""
            BUILD_COMMAND       ""
            INSTALL_COMMAND     ""
            TEST_COMMAND        ""
            )
	
	if (WIN32)
		include(cppkinMacro)
		LinuxPath_ToWinPath(${PROJECT_3RD_LOC} INSTALL_DIR_WIN)
		LinuxPath_ToWinPath(${PROJECT_3RD_LOC}/src/Spdlog SOURCE_DIR_WIN)

		ExternalProject_Add_Step(SpdLog SpdLog_Install_Headers
				COMMAND     if not exist ${INSTALL_DIR_WIN}\\include\\spdlog mkdir ${INSTALL_DIR_WIN}\\include\\spdlog && xcopy ${SOURCE_DIR_WIN}\\include\\spdlog ${INSTALL_DIR_WIN}\\include\\spdlog /E
				DEPENDEES   install
				)
	else()
		ExternalProject_Add_Step(SpdLog SpdLog_Install_Headers
				COMMAND     mkdir -p <INSTALL_DIR>/include && cp -r <SOURCE_DIR>/include/spdlog <INSTALL_DIR>/include/spdlog
				DEPENDEES   install
				)
	endif()

	set(CPPKIN_DEPEND_LIST ${CPPKIN_DEPEND_LIST} SpdLog)
    ExternalProject_Get_Property(SpdLog INSTALL_DIR)

    set (SPDLOG_ROOT_DIR          ${INSTALL_DIR})
    set (SPDLOG_INCLUDE_DIR       ${SPDLOG_ROOT_DIR}/include)
    set (SPDLOG_FOUND             YES)

    #    mark_as_advanced(GLOG_LIBRARY GLOG_INCLUDE_PATH)

endif ()
