if (NOT CURL_FOUND OR USE_BUNDLED_CURL)
	if (NOT CURL_VERSION_STRING OR USE_BUNDLED_CURL)
			set (CURL_VERSION_STRING        7.54.1)
			set (CURL_URL_MD5               21a6e5658fd55103a90b11de7b2a8a8c)
	endif ()
	
	if (WIN32)
		include(cppkinMacro)
		LinuxPath_ToWinPath(${CMAKE_CURRENT_SOURCE_DIR}/Third_Party INSTALL_DIR_WIN)
		LinuxPath_ToWinPath(${CMAKE_CURRENT_SOURCE_DIR}/Third_Party/src/CURL SOURCE_DIR_WIN)
		LinuxPath_ToWinPath(${CMAKE_CURRENT_SOURCE_DIR}/Third_Party/src/CURL-build/lib SOURCE_LIB_DIR_WIN)
		ExternalProject_Add(CURL
			URL                 https://curl.haxx.se/download/curl-${CURL_VERSION_STRING}.tar.gz
			URL_MD5             ${CURL_URL_MD5}
			CONFIGURE_COMMAND   cmake ${SOURCE_DIR_WIN} -G "Visual Studio 12" -DCMAKE_USE_OPENSSL=OFF -DHTTP_ONLY=ON -DUSE_NGHTTP2=OFF -DCURL_DISABLE_LDAP=ON -DCURL_STATICLIB=ON
			BUILD_COMMAND       cd ${SOURCE_LIB_DIR_WIN} && CMD /C msbuild libcurl.vcxproj /p:Configuration=Release
			INSTALL_COMMAND     ""
			TEST_COMMAND        ""
		)
		ExternalProject_Add_Step(CURL CURL_Create_HeadersDir
				COMMAND     if not exist ${INSTALL_DIR_WIN}\\include\\curl mkdir ${INSTALL_DIR_WIN}\\include\\curl
				DEPENDEES   install
				)
		ExternalProject_Add_Step(CURL CURL_Install_Headers
				COMMAND     xcopy ${SOURCE_DIR_WIN}\\include\\curl\\*.h ${INSTALL_DIR_WIN}\\include\\curl /E
				DEPENDEES   install
				)
		ExternalProject_Add_Step(CURL CURL_Create_Libs_Dir
				COMMAND     if not exist ${INSTALL_DIR_WIN}\\lib mkdir ${INSTALL_DIR_WIN}\\lib
				DEPENDEES   install
				)
		ExternalProject_Add_Step(CURL CURL_Install_Libs
				COMMAND     copy ${SOURCE_LIB_DIR_WIN}\\Release\\libcurl.lib ${INSTALL_DIR_WIN}\\lib
				DEPENDEES   install
				)
	else()
		set(debug_build "")
		if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
			set(debug_build --enable-debug)
        endif()
		ExternalProject_Add(CURL
			URL                 https://curl.haxx.se/download/curl-${CURL_VERSION_STRING}.tar.gz
			URL_MD5             ${CURL_URL_MD5}
			CONFIGURE_COMMAND   <SOURCE_DIR>/configure
									--prefix=<INSTALL_DIR>
									--without-ssl
									--without-nghttp2
									--disable-ldap
									${debug_build}
			BUILD_COMMAND       make
			INSTALL_COMMAND     make install
			TEST_COMMAND        ""
		)
	endif()

    ExternalProject_Get_Property(CURL INSTALL_DIR)

    set (CURL_ROOT_DIR          ${INSTALL_DIR})
    set (CURL_INCLUDE_DIRS      ${CURL_ROOT_DIR}/include)
    set (CURL_LIBRARY_DIRS      ${CURL_ROOT_DIR}/lib)
    set (CURL_LIBRARIES         ${CURL_LIBRARY_DIRS}/libcurl.a)
    set (CURL_FOUND             YES)

    mark_as_advanced(CURL_LIBRARIES CURL_INCLUDE_DIRS)

    message(STATUS "Use bundled curl v${CURL_VERSION_STRING}")
endif ()
