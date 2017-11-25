if (NOT BOOST_FOUND)
	if (WIN32)
		include(cppkinMacro)
		LinuxPath_ToWinPath(${CMAKE_CURRENT_SOURCE_DIR}/Third_Party INSTALL_DIR_WIN)
		LinuxPath_ToWinPath(${CMAKE_CURRENT_SOURCE_DIR}/Third_Party/src/Boost SOURCE_DIR_WIN)
		ExternalProject_Add(Boost
						DOWNLOAD_NAME       boost_1_65_1.tar.gz
						URL                 https://dl.bintray.com/boostorg/release/1.65.1/source/boost_1_65_1.tar.gz
						CONFIGURE_COMMAND   cd ${SOURCE_DIR_WIN} && CMD /C bootstrap
						BUILD_COMMAND       cd ${SOURCE_DIR_WIN} && .\\b2 install --prefix=${INSTALL_DIR_WIN} --exec-prefix=${INSTALL_DIR_WIN}
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
						CONFIGURE_COMMAND   cd <SOURCE_DIR> && ./bootstrap.sh --prefix=<INSTALL_DIR> --exec-prefix=<INSTALL_DIR>
						BUILD_COMMAND       ""
						INSTALL_COMMAND     cd <SOURCE_DIR> && ./b2 install 
						TEST_COMMAND        ""
		)
	endif()

ExternalProject_Get_Property(Boost INSTALL_DIR)

set (BOOST_ROOT_DIR          ${INSTALL_DIR})
set (BOOST_INCLUDE_DIR       ${INSTALL_DIR}/include)
set (BOOST_FOUND             YES)

endif ()
