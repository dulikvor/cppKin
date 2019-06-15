if (NOT Boost_FOUND)
	string(TOLOWER ${CMAKE_BUILD_TYPE} variant)
	if (WIN32)
		if(${VC_VERSION} STREQUAL "12")
			set(toolsetvalue msvc-12.0)
		else(${VC_VERSION} STREQUAL "14")
			set(toolsetvalue msvc-14.0)
		endif()
		include(cppkinMacro)
		LinuxPath_ToWinPath(${PROJECT_3RD_LOC} INSTALL_DIR_WIN)
		LinuxPath_ToWinPath(${PROJECT_3RD_LOC}/src/Boost SOURCE_DIR_WIN)
		ExternalProject_Add(Boost
						DOWNLOAD_NAME       boost_1_65_1.tar.gz
						URL                 https://dl.bintray.com/boostorg/release/1.65.1/source/boost_1_65_1.tar.gz
						URL_MD5             ee64fd29a3fe42232c6ac3c419e523cf
						CONFIGURE_COMMAND   cd ${SOURCE_DIR_WIN} && CMD /C bootstrap
						BUILD_COMMAND       cd ${SOURCE_DIR_WIN} && .\\b2 install --prefix=${INSTALL_DIR_WIN} --exec-prefix=${INSTALL_DIR_WIN} --with-program_options toolset=${toolsetvalue} -j4 link=shared threading=multi runtime-link=shared variant=${variant}
						INSTALL_COMMAND     ""
						TEST_COMMAND        ""
		)
		ExternalProject_Add_Step(Boost Boost_Install_Headers
				COMMAND     xcopy ${INSTALL_DIR_WIN}\\include\\boost-1_65_1 ${INSTALL_DIR_WIN}\\include /E && rd /s /q "${INSTALL_DIR_WIN}\\include\\boost-1_65_1"
				DEPENDEES   install
				)
	else()
		ExternalProject_Add(Boost
						DOWNLOAD_NAME       boost_1_65_1.tar.gz
						URL                 https://dl.bintray.com/boostorg/release/1.65.1/source/boost_1_65_1.tar.gz
						URL_MD5             ee64fd29a3fe42232c6ac3c419e523cf
						CONFIGURE_COMMAND   cd <SOURCE_DIR> && ./bootstrap.sh --prefix=<INSTALL_DIR> --exec-prefix=<INSTALL_DIR>
						BUILD_COMMAND       ""
						INSTALL_COMMAND     cd <SOURCE_DIR> && ./b2 install --with-program_options --layout=system link=shared threading=multi runtime-link=shared variant=${variant}
						TEST_COMMAND        ""
		)
	endif()
    set(CPPKIN_DEPEND_LIST ${CPPKIN_DEPEND_LIST} Boost)

ExternalProject_Get_Property(Boost INSTALL_DIR)

set (Boost_ROOT_DIR          ${INSTALL_DIR})
set (Boost_INCLUDE_DIR       ${INSTALL_DIR}/include)
set (Boost_FOUND             YES)
set (Boost_MAJOR_VERSION     1)
set (Boost_MINOR_VERSION     65)
set (Boost_SUBMINOR_VERSION  1)

endif ()
