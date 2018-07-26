if (NOT Boost_FOUND)
	if (WIN32)
		include(cppkinMacro)
		LinuxPath_ToWinPath(${CMAKE_CURRENT_SOURCE_DIR}/Third_Party INSTALL_DIR_WIN)
		LinuxPath_ToWinPath(${CMAKE_CURRENT_SOURCE_DIR}/Third_Party/src/Boost SOURCE_DIR_WIN)
		ExternalProject_Add(Boost
						DOWNLOAD_NAME       boost_1_65_1.tar.gz
						URL                 https://dl.bintray.com/boostorg/release/1.65.1/source/boost_1_65_1.tar.gz
						URL_MD5             ee64fd29a3fe42232c6ac3c419e523cf
						CONFIGURE_COMMAND   cd ${SOURCE_DIR_WIN} && CMD /C bootstrap
						BUILD_COMMAND       cd ${SOURCE_DIR_WIN} && .\\b2 install --prefix=${INSTALL_DIR_WIN} --exec-prefix=${INSTALL_DIR_WIN} --with-program_options toolset=msvc-12.0 -j4 link=shared threading=multi runtime-link=shared
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
						INSTALL_COMMAND     cd <SOURCE_DIR> && ./b2 install --with-program_options --layout=system link=shared threading=multi runtime-link=shared
						TEST_COMMAND        ""
		)
	endif()

ExternalProject_Get_Property(Boost INSTALL_DIR)

set (Boost_ROOT_DIR          ${INSTALL_DIR})
set (Boost_INCLUDE_DIR       ${INSTALL_DIR}/include)
set (Boost_FOUND             YES)

endif ()