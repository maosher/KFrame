
:  build log4cxx

"%VS140COMNTOOLS%..\IDE\Devenv" uv.sln /build "Debug|X64"
copy /y .\Debug\lib\libuv.lib ..\..\_lib\win64\libuvd.lib


"%VS140COMNTOOLS%..\IDE\Devenv" uv.sln /build "Release|X64"
copy /y .\Release\lib\libuv.lib ..\..\_lib\win64\libuv.lib
