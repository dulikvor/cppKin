set PROJECT_DIR=%1
set THIRD_PARTY_DIR=%2
set COMPILER=%THIRD_PARTY_DIR%\bin\thrift.exe
set COMPILE_DIR=IDL
set OUTPUT_DIR=GeneratedFiles
#Create output dir if not exists
mkdir %PROJECT_DIR%\%OUTPUT_DIR%
ECHO "Thrift build begins!!!"
FOR %%I IN (%PROJECT_DIR%\%COMPILE_DIR%\*.thrift) DO (
	CMD /C %COMPILER% --gen cpp --out %PROJECT_DIR%\%OUTPUT_DIR% %%I
	)
