if (NOT RAPIDJSON_FOUND)
    if (WIN32)
		include(cppkinMacro)
		LinuxPath_ToWinPath(${PROJECT_3RD_LOC} INSTALL_DIR_WIN)
		LinuxPath_ToWinPath(${PROJECT_3RD_LOC}/src/RAPIDJSON SOURCE_DIR_WIN)
		ExternalProject_Add(RAPIDJSON
                        DOWNLOAD_NAME       RapidJSON-1.1.0.tar.gz
                        URL                 https://github.com/miloyip/rapidjson/archive/v1.1.0.tar.gz
						BUILD_COMMAND       if not exist ${INSTALL_DIR_WIN}\\include\\rapidjson mkdir ${INSTALL_DIR_WIN}\\include\\rapidjson
                        INSTALL_COMMAND     xcopy ${SOURCE_DIR_WIN}\\include\\rapidjson\\*.h ${INSTALL_DIR_WIN}\\include\\rapidjson /sy
						TEST_COMMAND        ""
		)
    else()
        ExternalProject_Add(RAPIDJSON
                DOWNLOAD_NAME       RapidJSON-1.1.0.tar.gz
                URL                 https://github.com/miloyip/rapidjson/archive/v1.1.0.tar.gz
				BUILD_COMMAND       ""
				INSTALL_COMMAND     mkdir -p <INSTALL_DIR>/include && cp -a <SOURCE_DIR>/include/rapidjson <INSTALL_DIR>/include/rapidjson
                TEST_COMMAND        ""
                )
    endif()

	set(CPPKIN_DEPEND_LIST ${CPPKIN_DEPEND_LIST} RAPIDJSON)
    ExternalProject_Get_Property(RAPIDJSON INSTALL_DIR)

    set (RAPIDJSON_ROOT_DIR          ${INSTALL_DIR})
    set (RAPIDJSON_INCLUDE_DIR       ${INSTALL_DIR}/include)
    set (RAPIDJSON_FOUND             YES)

endif ()

