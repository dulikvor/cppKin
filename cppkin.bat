@echo off
setlocal EnableDelayedExpansion
ECHO "#########Starting to build#########"
IF "%1" == "install" (call:clean && call:install %*) ELSE ^
IF "%1" == "--help" (call:display_help) ELSE (ECHO "supported commands - --help, install")
exit /b 0
:display_help
    ECHO "############################cppkin help############################"
    ECHO "#--with_thrift              ## Will dictate if cppkin supports    #"
    ECHO "#                           ## the thrift transportation layer.   #"
    ECHO "#-----------------------------------------------------------------#"
    ECHO "#--with_examples            ## Will compile cppkin examples       #"
    ECHO "###################################################################"
goto:eof
:install
    SET WITH_THRIFT=OFF
    SET WITH_TESTS=OFF
    SET WITH_EXAMPLES=OFF
	shift
	:FOR_LABEL
	IF "%1" == "--with_thrift"   (SET WITH_THRIFT=ON) ELSE ^
	IF "%1" == "--with_tests"    (ECHO "--with_tests is not supported in windows")  ELSE ^
	IF "%1" == "--with_examples" (SET WITH_EXAMPLES=ON)
	shift
	IF NOT [%1]==[] GOTO FOR_LABEL
	IF %WITH_THRIFT%==ON (
	cmake -G "Visual Studio 12" -DCMAKE_BUILD_TYPE=Release -D3RD_PARTY_INSTALL_STEP=ON -DPRE_COMPILE_STEP=ON -DWITH_THRIFT=%WITH_THRIFT% -DWITH_EXAMPLES=%WITH_EXAMPLES%
	IF exist Thrift.vcxproj (CMD /C msbuild Thrift.vcxproj /property:Configuration=Release)
	IF exist THRIFT_BUILD.vcxproj (CMD /C msbuild THRIFT_BUILD.vcxproj /property:Configuration=Release)
	call:clean
	)
	cmake -G "Visual Studio 12" -DCMAKE_BUILD_TYPE=Release -D3RD_PARTY_INSTALL_STEP=ON -DCOMPILATION_STEP=ON -DWITH_THRIFT=%WITH_THRIFT% -DWITH_EXAMPLES=%WITH_EXAMPLES%
	IF exist POCO.vcxproj (CMD /C msbuild POCO.vcxproj /property:Configuration=Release)
	IF exist CURL.vcxproj (CMD /C msbuild CURL.vcxproj /property:Configuration=Release)
	IF exist SpdLog.vcxproj (CMD /C msbuild SpdLog.vcxproj /property:Configuration=Release)
	IF exist Core.vcxproj (CMD /C msbuild Core.vcxproj /property:Configuration=Release)
	IF exist Boost.vcxproj (CMD /C msbuild Boost.vcxproj /property:Configuration=Release)
	IF %WITH_EXAMPLES%==ON (IF exist examples/example.vcxproj (CMD /C msbuild examples/example.vcxproj /property:Configuration=Release))
	IF exist cppkin.vcxproj (CMD /C msbuild cppkin.vcxproj /property:Configuration=Release)
goto:eof
:clean
	del "*.vcxproj"
	del "*.vcxproj.filters"
	RMDIR /S /Q CMakeFiles
	del CMakeCache.txt
goto:eof
