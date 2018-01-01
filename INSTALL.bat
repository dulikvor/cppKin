ECHO "Installation under way"
cmake -G "Visual Studio 12" -D3RD_PARTY_INSTALL_STEP=ON -DPRE_COMPILE_STEP=ON -DCOMPILATION_STEP=ON -DSPDLOG_SUPPORT=ON
CMD /C msbuild SpdLog.vcxproj /p:Configuration=Release
CMD /C msbuild Boost.vcxproj /p:Configuration=Release
CMD /C msbuild Thrift.vcxproj /p:Configuration=Release
CMD /C msbuild CURL.vcxproj /p:Configuration=Release
CMD /C msbuild THRIFT_BUILD.vcxproj /p:Configuration=Release
CMD /C msbuild cppkin.vcxproj /p:Configuration=Release