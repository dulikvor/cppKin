@echo off
setlocal EnableDelayedExpansion
IF "%1" == "install" ( call :clean && call :install %* 
) ELSE IF "%1" == "--help" ( call :display_help
) ELSE ( ECHO "supported commands - --help, install" )
exit /b 0

:install
    SET WITH_THRIFT=OFF
    SET WITH_TESTS=OFF
    SET WITH_EXAMPLES=OFF
    SET BUILD_TYPE=Release
	shift
	:FOR_LABEL
	IF "%1" == "--with_thrift" (SET WITH_THRIFT=ON
	) ELSE IF "%1" == "--with_tests" (ECHO "--with_tests is not supported in windows"
	)  ELSE IF "%1" == "--with_examples" (SET WITH_EXAMPLES=ON
    )  ELSE IF "%1" == "--debug" (SET BUILD_TYPE=Debug)
	shift
    IF NOT [%1]==[] GOTO FOR_LABEL
	IF %WITH_THRIFT%==ON (
	cmake -G "Visual Studio 12" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -D3RD_PARTY_INSTALL_STEP=ON -DPRE_COMPILE_STEP=ON -DWITH_THRIFT=%WITH_THRIFT% -DWITH_EXAMPLES=%WITH_EXAMPLES%
    IF exist Boost.vcxproj (CMD /C msbuild Boost.vcxproj /property:Configuration=%BUILD_TYPE%)
	IF exist Thrift.vcxproj (CMD /C msbuild Thrift.vcxproj /property:Configuration=%BUILD_TYPE%)
	IF exist THRIFT_BUILD.vcxproj (CMD /C msbuild THRIFT_BUILD.vcxproj /property:Configuration=%BUILD_TYPE%)
	call:clean
	)
	cmake -G "Visual Studio 12" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -D3RD_PARTY_INSTALL_STEP=ON -DCOMPILATION_STEP=ON -DWITH_THRIFT=%WITH_THRIFT% -DWITH_EXAMPLES=%WITH_EXAMPLES%
    IF %WITH_THRIFT%==OFF ( IF exist Boost.vcxproj (CMD /C msbuild Boost.vcxproj /property:Configuration=%BUILD_TYPE%) )
    cmake -G "Visual Studio 12" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -D3RD_PARTY_INSTALL_STEP=ON -DCOMPILATION_STEP=ON -DWITH_THRIFT=%WITH_THRIFT% -DWITH_EXAMPLES=%WITH_EXAMPLES%
	IF exist POCO.vcxproj (CMD /C msbuild POCO.vcxproj /property:Configuration=%BUILD_TYPE%)
	IF exist CURL.vcxproj (CMD /C msbuild CURL.vcxproj /property:Configuration=%BUILD_TYPE%)
	IF exist SpdLog.vcxproj (CMD /C msbuild SpdLog.vcxproj /property:Configuration=%BUILD_TYPE%)
	IF exist Core.vcxproj (CMD /C msbuild Core.vcxproj /property:Configuration=%BUILD_TYPE%)
	IF exist cppkin.vcxproj (CMD /C msbuild cppkin.vcxproj /property:Configuration=%BUILD_TYPE%)
    IF %WITH_EXAMPLES%==ON (IF exist examples/example.vcxproj (CMD /C msbuild examples/example.vcxproj /property:Configuration=%BUILD_TYPE%))
goto :eof

:display_help
    ECHO "############################cppkin help############################"
    ECHO "#--with_thrift              ## Will dictate if cppkin supports    #"
    ECHO "#                           ## the thrift transportation layer.   #"
    ECHO "#-----------------------------------------------------------------#"
    ECHO "#--with_examples            ## Will compile cppkin examples       #"
    ECHO "#-----------------------------------------------------------------#"
    ECHO "#--debug                    ## debug build                        #"
    ECHO "###################################################################"
goto :eof

:clean
	del "*.vcxproj"
	del "*.vcxproj.filters"
	RMDIR /S /Q CMakeFiles
	del CMakeCache.txt
goto :eof
