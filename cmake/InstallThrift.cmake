if (NOT THRIFT_FOUND)
	if (WIN32)
		include(cppkinMacro)
		LinuxPath_ToWinPath(${CMAKE_CURRENT_SOURCE_DIR}/Third_Party INSTALL_DIR_WIN)
		LinuxPath_ToWinPath(${CMAKE_CURRENT_SOURCE_DIR}/Third_Party/src/Thrift SOURCE_DIR_WIN)
		ExternalProject_Add(Thrift
				DOWNLOAD_NAME       thrift-0.10.0.tar.gz
				URL                 http://archive.apache.org/dist/thrift/0.10.0/thrift-0.10.0.tar.gz
				CONFIGURE_COMMAND   copy ${INSTALL_DIR_WIN}\\Patch\\FindBoost.cmake ${SOURCE_DIR_WIN}\\build\\cmake && copy ${INSTALL_DIR_WIN}\\Patch\\Thrift.h ${SOURCE_DIR_WIN}\\lib\\cpp\\src\\thrift && cd ${SOURCE_DIR_WIN} && cmake -G "Visual Studio 12" -DWITH_SHARED_LIB=off -DBUILD_TUTORIALS=OFF -DBUILD_TESTING=OFF
				BUILD_COMMAND       cd ${SOURCE_DIR_WIN} && msbuild ALL_BUILD.vcxproj /p:Configuration=Release
				INSTALL_COMMAND     ""
				TEST_COMMAND        ""
				)
		ExternalProject_Add_Step(Thrift Thrift_Create_HeadersDir
				COMMAND     if not exist ${INSTALL_DIR_WIN}\\include\\thrift mkdir ${INSTALL_DIR_WIN}\\include\\thrift
				DEPENDEES   install
				)
		ExternalProject_Add_Step(Thrift Thrift_Install_Headers
				COMMAND     xcopy ${SOURCE_DIR_WIN}\\lib\\cpp\\src\\thrift\\*.h ${INSTALL_DIR_WIN}\\include\\thrift /E
				DEPENDEES   install
				)
		ExternalProject_Add_Step(Thrift Thrift_Install_tcc_Files
				COMMAND     xcopy ${SOURCE_DIR_WIN}\\lib\\cpp\\src\\thrift\\*.tcc ${INSTALL_DIR_WIN}\\include\\thrift /E
				DEPENDEES   install
				)
		ExternalProject_Add_Step(Thrift Thrift_Create_Binaries_Dir
				COMMAND     if not exist ${INSTALL_DIR_WIN}\\bin mkdir ${INSTALL_DIR_WIN}\\bin
				DEPENDEES   install
				)
		ExternalProject_Add_Step(Thrift Thrift_Install_Binaries
				COMMAND     copy ${SOURCE_DIR_WIN}\\bin\\Release\\thrift.exe ${INSTALL_DIR_WIN}\\bin
				DEPENDEES   install
				)
		ExternalProject_Add_Step(Thrift Thrift_Create_Libs_Dir
				COMMAND     if not exist ${INSTALL_DIR_WIN}\\lib mkdir ${INSTALL_DIR_WIN}\\lib
				DEPENDEES   install
				)
		ExternalProject_Add_Step(Thrift Thrift_Install_Libs
				COMMAND     copy ${SOURCE_DIR_WIN}\\lib\\Release\\thriftmd.lib ${INSTALL_DIR_WIN}\\lib
				DEPENDEES   install
				)
	else()
		ExternalProject_Add(Thrift
				DOWNLOAD_NAME       thrift-0.10.0.tar.gz
				URL                 http://archive.apache.org/dist/thrift/0.10.0/thrift-0.10.0.tar.gz
				CONFIGURE_COMMAND   cd <SOURCE_DIR> && ./configure --prefix=<INSTALL_DIR> --exec-prefix=<INSTALL_DIR>
				--without-qt4
				--without-qt5
				--without-c_glib
				--without-csharp
				--without-java
				--without-erlang
				--without-nodejs
				--without-lua
				--without-python
				--without-perl
				--without-php
				--without-php_extension
				--without-dart
				--without-ruby
				--without-haskell
				--without-go
				--without-haxe
				--without-d
				BUILD_COMMAND       make -C <SOURCE_DIR>
				INSTALL_COMMAND     make -C <SOURCE_DIR> install
				TEST_COMMAND        ""
				)
	endif()

    ExternalProject_Get_Property(Thrift INSTALL_DIR)

    set (THRIFT_ROOT_DIR          ${INSTALL_DIR})
    set (THRIFT_INCLUDE_DIR       ${THRIFT_ROOT_DIR}/include)
    set (THRIFT_BINARY_DIR        ${THRIFT_ROOT_DIR}/bin)
    set (THRIFT_FOUND             YES)

endif ()
