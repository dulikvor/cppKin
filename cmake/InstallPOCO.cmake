if (NOT POCO_FOUND)
    if (WIN32)
		include(cppkinMacro)
		LinuxPath_ToWinPath(${CMAKE_CURRENT_SOURCE_DIR}/Third_Party INSTALL_DIR_WIN)
		LinuxPath_ToWinPath(${CMAKE_CURRENT_SOURCE_DIR}/Third_Party/src/POCO SOURCE_DIR_WIN)
		ExternalProject_Add(POCO
						DOWNLOAD_NAME       poco-1.8.1-release.tar.gz
						URL                 https://github.com/pocoproject/poco/archive/poco-1.8.1-release.tar.gz
						URL_MD5             e64735b06d2520ebf1bcc2b7ffe81562
						CONFIGURE_COMMAND   cd ${SOURCE_DIR_WIN} && echo Foundation > components && echo CppUnit >> components && echo JSON >> components
						BUILD_COMMAND       cd ${SOURCE_DIR_WIN} && CMD /C buildwin.cmd 120 build shared release Win32 nosamples notests msbuild
						INSTALL_COMMAND     if not exist ${INSTALL_DIR_WIN}\\include\\Poco\\JSON mkdir ${INSTALL_DIR_WIN}\\include\\Poco\\JSON && if not exist ${INSTALL_DIR_WIN}\\lib mkdir ${INSTALL_DIR_WIN}\\lib
						TEST_COMMAND        ""
		)
				
		ExternalProject_Add_Step(POCO POCO_Install_JSON_Headers
				COMMAND     xcopy ${SOURCE_DIR_WIN}\\JSON\\include\\Poco\\JSON\\*.h ${INSTALL_DIR_WIN}\\include\\Poco\\JSON /E
				DEPENDEES   install
				)
				
		ExternalProject_Add_Step(POCO POCO_Install_Foundation_Headers
				COMMAND     xcopy ${SOURCE_DIR_WIN}\\Foundation\\include\\Poco\\*.h ${INSTALL_DIR_WIN}\\include\\Poco /E
				DEPENDEES   install
				)
				
		ExternalProject_Add_Step(POCO POCO_Install_Libs
				COMMAND     xcopy ${SOURCE_DIR_WIN}\\lib\\*.lib ${INSTALL_DIR_WIN}\\lib /E && xcopy ${SOURCE_DIR_WIN}\\bin\\*.dll ${INSTALL_DIR_WIN}\\lib /E
				DEPENDEES   install
				)
				
    else()
        ExternalProject_Add(POCO
                DOWNLOAD_NAME       poco-1.8.1-release.tar.gz
                URL                 https://github.com/pocoproject/poco/archive/poco-1.8.1-release.tar.gz
                URL_MD5             e64735b06d2520ebf1bcc2b7ffe81562
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

