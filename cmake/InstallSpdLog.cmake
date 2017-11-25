if (NOT SPDLOG_FOUND)

    ExternalProject_Add(SpdLog
            DOWNLOAD_NAME       spdlog-0.13.0.tar
            URL                 https://github.com/gabime/spdlog/archive/v0.13.0.tar.gz
            CONFIGURE_COMMAND   ""
            BUILD_COMMAND       ""
            INSTALL_COMMAND     ""
            TEST_COMMAND        ""
            )
	
	if (WIN32)
		macro(msys_to_cmake_path MsysPath ResultingPath)
			string(REPLACE "/" "\\" ${ResultingPath} "${MsysPath}")
		endmacro()
		msys_to_cmake_path(${CMAKE_CURRENT_SOURCE_DIR}/Third_Party INSTALL_DIR_WIN)
		msys_to_cmake_path(${CMAKE_CURRENT_SOURCE_DIR}/Third_Party/src/Spdlog SOURCE_DIR_WIN)

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

    ExternalProject_Get_Property(SpdLog INSTALL_DIR)

    set (SPDLOG_ROOT_DIR          ${INSTALL_DIR})
    set (SPDLOG_INCLUDE_DIR       ${SPDLOG_ROOT_DIR}/include)
    set (SPDLOG_FOUND             YES)

    #    mark_as_advanced(GLOG_LIBRARY GLOG_INCLUDE_PATH)

endif ()
