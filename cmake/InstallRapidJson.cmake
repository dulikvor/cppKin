if (NOT RAPIDJSON_FOUND)
    if (WIN32)
		include(cppkinMacro)
		LinuxPath_ToWinPath(${PROJECT_3RD_LOC} INSTALL_DIR_WIN)
		LinuxPath_ToWinPath(${PROJECT_3RD_LOC}/src/RAPIDJSON SOURCE_DIR_WIN)
		string(TOLOWER ${CMAKE_BUILD_TYPE} poco_build)
		ExternalProject_Add(RAPIDJSON
						DOWNLOAD_NAME       poco-1.8.1-release.tar.gz
						URL                 https://github.com/pocoproject/poco/archive/poco-1.8.1-release.tar.gz
						URL_MD5             e64735b06d2520ebf1bcc2b7ffe81562
						CONFIGURE_COMMAND   cd ${SOURCE_DIR_WIN} && echo Foundation > components && echo CppUnit >> components && echo JSON >> components
						BUILD_COMMAND       cd ${SOURCE_DIR_WIN} && CMD /C buildwin.cmd 120 build shared ${poco_build} Win32 nosamples notests msbuild
						INSTALL_COMMAND     if not exist ${INSTALL_DIR_WIN}\\include\\Poco\\JSON mkdir ${INSTALL_DIR_WIN}\\include\\Poco\\JSON && if not exist ${INSTALL_DIR_WIN}\\lib mkdir ${INSTALL_DIR_WIN}\\lib
						TEST_COMMAND        ""
		)
				
		ExternalProject_Add_Step(RAPIDJSON RAPIDJSON_Install_JSON_Headers
				COMMAND     xcopy ${SOURCE_DIR_WIN}\\JSON\\include\\Poco\\JSON\\*.h ${INSTALL_DIR_WIN}\\include\\Poco\\JSON /E
				DEPENDEES   install
				)
    else()
        ExternalProject_Add(RAPIDJSON
                DOWNLOAD_NAME       RapidJSON-1.1.0.tar.gz
                URL                 https://github.com/miloyip/rapidjson/archive/v1.1.0.tar.gz
				BUILD_COMMAND       ""
				INSTALL_COMMAND     cp -a <SOURCE_DIR>/include/rapidjson <INSTALL_DIR>/include/rapidjson
                TEST_COMMAND        ""
                )
    endif()

    ExternalProject_Get_Property(RAPIDJSON INSTALL_DIR)

    set (RAPIDJSON_ROOT_DIR          ${INSTALL_DIR})
    set (RAPIDJSON_INCLUDE_DIR       ${INSTALL_DIR}/include)
    set (RAPIDJSON_FOUND             YES)

endif ()

