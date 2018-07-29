if (NOT Core_FOUND)
	if (WIN32)
		include(cppkinMacro)
		LinuxPath_ToWinPath(${CMAKE_CURRENT_SOURCE_DIR}/Third_Party INSTALL_DIR_WIN)
		LinuxPath_ToWinPath(${CMAKE_CURRENT_SOURCE_DIR}/Third_Party/src/Core SOURCE_DIR_WIN)
		
		ExternalProject_Add(Core
				URL                 https://github.com/Dudi119/Core/archive/v1.0.1.tar.gz
				CONFIGURE_COMMAND   cd ${SOURCE_DIR_WIN} && cmake -G "Visual Studio 12" -DCORE_3RD_PARTY_DIR:STRING=${INSTALL_DIR_WIN} -DCORE_SPDLOG_SUPPORT=OFF -DCORE_COMPILE_STEP=ON .
				BUILD_COMMAND       cd ${SOURCE_DIR_WIN} && CMD /C msbuild Core.vcxproj /p:Configuration=Release
				INSTALL_COMMAND     ""
				TEST_COMMAND        ""
				)
				
		ExternalProject_Add_Step(Core Core_Create_HeadersDir
				COMMAND     if not exist ${INSTALL_DIR_WIN}\\include\\core mkdir ${INSTALL_DIR_WIN}\\include\\core
				DEPENDEES   install
				)
		ExternalProject_Add_Step(Core Core_Install_Headers
				COMMAND     xcopy ${SOURCE_DIR_WIN}\\src\\*.h ${INSTALL_DIR_WIN}\\include\\core /E
				DEPENDEES   install
				)

		ExternalProject_Add_Step(Core Core_Create_Libs_Dir
				COMMAND     if not exist ${INSTALL_DIR_WIN}\\lib mkdir ${INSTALL_DIR_WIN}\\lib
				DEPENDEES   install
				)
		ExternalProject_Add_Step(Core Core_Install_Libs
				COMMAND     copy ${SOURCE_DIR_WIN}\\Release\\Core.dll ${INSTALL_DIR_WIN}\\lib && copy ${SOURCE_DIR_WIN}\\Release\\Core.lib ${INSTALL_DIR_WIN}\\lib
				DEPENDEES   install
				)
	else()
		ExternalProject_Add(Core
				URL                 https://github.com/Dudi119/Core/archive/v1.0.1.tar.gz
				CONFIGURE_COMMAND   cd <SOURCE_DIR> && cmake -DCORE_3RD_PARTY_DIR:STRING=<INSTALL_DIR> -DCORE_SPDLOG_SUPPORT=OFF -DCORE_COMPILE_STEP=ON .
				BUILD_COMMAND       cd <SOURCE_DIR> && make
				INSTALL_COMMAND     mkdir -p <INSTALL_DIR>/lib && cp <SOURCE_DIR>/bin/libCore.so <INSTALL_DIR>/lib
				TEST_COMMAND        ""
				)
				
		ExternalProject_Add_Step(Core Core_Install_Headers
		COMMAND     mkdir -p <INSTALL_DIR>/include/core && sh -c "cp <SOURCE_DIR>/src/*.h <INSTALL_DIR>/include/core/"
		DEPENDEES   install
		)

    ExternalProject_Get_Property(Core INSTALL_DIR)
    add_custom_target(Core_stub)
		

	endif ()
	
	ExternalProject_Get_Property(Core INSTALL_DIR)

	set (Core_ROOT_DIR          ${INSTALL_DIR})
	set (Core_INCLUDE_DIR       ${Core_ROOT_DIR}/include)
	set (Core_LIBRARY_DIR       ${Core_ROOT_DIR}/lib)
	set (Core_FOUND             YES)
endif ()
