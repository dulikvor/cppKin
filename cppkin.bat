@echo off
setlocal EnableDelayedExpansion
for %%F in (%0) do set scriptdir=%%~dpF
IF "%1" == "install" ( call :install %* 
) ELSE IF "%1" == "--help" ( call :display_help
) ELSE ( ECHO "supported commands - --help, install" )
IF %ERRORLEVEL% GEQ 1 (EXIT /B 2) ELSE (EXIT /B 0)

:install
    SET WITH_TESTS=OFF
    SET WITH_EXAMPLES=OFF
    SET BUILD_TYPE=Release
	SET VCPP_VERSION=12
    SET THIRD_PARTY_PREFIX=""
    SET OUTPUT_DIR=""
	shift
	:FOR_LABEL
	IF "%1" == "--with_tests" (ECHO "--with_tests is not supported in windows"
	)  ELSE IF "%1" == "--with_examples" (SET WITH_EXAMPLES=ON
    )  ELSE IF "%1" == "--debug" (SET BUILD_TYPE=Debug
    )  ELSE IF "%1" == "--3rd_loc_prefix" (SET THIRD_PARTY_PREFIX=%2 && shift
    )  ELSE IF "%1" == "--output_dir" (SET OUTPUT_DIR=%2 && shift
	)  ELSE IF "%1" == "--vc_version" (SET VCPP_VERSION=%2&& shift)
	shift
    IF NOT [%1]==[] GOTO FOR_LABEL
    cmake %scriptdir% -G "Visual Studio %VCPP_VERSION%" -DVC_VERSION=%VCPP_VERSION% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -D3RD_PARTY_INSTALL_STEP=ON -DCOMPILATION_STEP=ON -DWITH_EXAMPLES=%WITH_EXAMPLES% -DPROJECT_3RD_LOC:STRING=%THIRD_PARTY_PREFIX% -DOUTPUT_DIR:STRING=%OUTPUT_DIR%
    IF exist Boost.vcxproj (CMD /C msbuild Boost.vcxproj /property:Configuration=%BUILD_TYPE%)
	IF exist POCO.vcxproj (CMD /C msbuild POCO.vcxproj /property:Configuration=%BUILD_TYPE%)
	IF exist CURL.vcxproj (CMD /C msbuild CURL.vcxproj /property:Configuration=%BUILD_TYPE%)
	IF exist SpdLog.vcxproj (CMD /C msbuild SpdLog.vcxproj /property:Configuration=%BUILD_TYPE%)
	IF exist Core.vcxproj (CMD /C msbuild Core.vcxproj /property:Configuration=%BUILD_TYPE%)
	IF exist RAPIDJSON.vcxproj (CMD /C msbuild RAPIDJSON.vcxproj /property:Configuration=%BUILD_TYPE%)
	IF exist cppkin.vcxproj (CMD /C msbuild cppkin.vcxproj /property:Configuration=%BUILD_TYPE%)
    IF %WITH_EXAMPLES%==ON (IF exist examples/cpp/example.vcxproj (CMD /C msbuild examples/cpp/example.vcxproj /property:Configuration=%BUILD_TYPE%))
	IF NOT %OUTPUT_DIR% == "" (IF exist INSTALL.vcxproj (CMD /C msbuild INSTALL.vcxproj /property:Configuration=%BUILD_TYPE%))
goto :eof

:display_help
    ECHO.
    ECHO cppkin install [options]...
    ECHO Install                       Install related options.
    ECHO =======
    ECHO --3rd_loc_prefix=^<PREFIX^>     3rd party install area prefix.   
    ECHO.                                                               
    ECHO --output_dir=^<DIR^>            cppkin installation path, in relation to the batch script location.
    ECHO.                                                                
	ECHO --vc_version=^<Number^>         12 (Default) = Visual Studio 2013,
	ECHO.                              14 = Visual Studio 2015 
    ECHO.                                                                
    ECHO --debug                       debug build, otherwise - release.
    ECHO.                                                                
    ECHO Other options                                                  
    ECHO =============                                                        
    ECHO.                                                                
    ECHO --with_examples               Compile cppkin examples
goto :eof

:clean
	del "*.vcxproj"
	del "*.vcxproj.filters"
	RMDIR /S /Q CMakeFiles
	del CMakeCache.txt
goto :eof
