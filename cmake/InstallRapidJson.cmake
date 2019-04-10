if (NOT RAPIDJSON_FOUND)
    if (WIN32)
		include(cppkinMacro)
		LinuxPath_ToWinPath(${PROJECT_3RD_LOC} INSTALL_DIR_WIN)
		LinuxPath_ToWinPath(${PROJECT_3RD_LOC}/src/RAPIDJSON SOURCE_DIR_WIN)
		string(TOLOWER ${CMAKE_BUILD_TYPE} poco_build)
		ExternalProject_Add(RAPIDJSON
                        DOWNLOAD_NAME       RapidJSON-1.1.0.tar.gz
                        URL                 https://github.com/miloyip/rapidjson/archive/v1.1.0.tar.gz
						CONFIGURE_COMMAND   cd ${SOURCE_DIR_WIN} && echo Foundation > components && echo CppUnit >> components && echo JSON >> components
						BUILD_COMMAND       if not exist ${INSTALL_DIR_WIN}\\include\\rapidjson mkdir ${INSTALL_DIR_WIN}\\include\\rapidjson
                        INSTALL_COMMAND     xcopy ${SOURCE_DIR_WIN}\\include\\rapidjson\\*.h ${INSTALL_DIR_WIN}\\include\\rapidjson /sy
						TEST_COMMAND        ""
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

