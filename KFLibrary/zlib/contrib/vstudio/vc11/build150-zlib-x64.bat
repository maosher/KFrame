﻿
:  build zlib


"%VS150COMNTOOLS%..\IDE\Devenv" zlibvc.sln /rebuild "Debug|X64" /project zlibstat
copy /y .\x64\ZlibStatDebug\zlibstat.lib ..\..\..\..\..\_lib\win64\debug\3rd\zlibd.lib


"%VS150COMNTOOLS%..\IDE\Devenv" zlibvc.sln /rebuild "Release|X64" /project zlibstat
copy /y .\x64\ZlibStatRelease\zlibstat.lib ..\..\..\..\..\_lib\win64\release\3rd\zlib.lib
