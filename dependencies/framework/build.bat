@echo off

call "%VS140COMNTOOLS%"\vsvars32.bat

call:build COFF_tools\dotNET\COFF_tools.sln "Any CPU" Debug
call:build CUDA_build_tools\embedCUDA\source\embedCUDA.sln "Any CPU" Debug
call:build GLSL_build_tools\build\vs2015\GLSL_build_tools.sln x64 Debug

REM call:build Win32_core_tools\build\vs2015\Win32_core_tools.vcxproj Win32 Debug Release
call:build Win32_core_tools\build\vs2015\Win32_core_tools.vcxproj x64 Debug Release

REM call:build GL_platform_tools\build\vs2015\glcore\glcore.vcxproj Win32 Debug "Debug DLL" Release "Release DLL"
call:build GL_platform_tools\build\vs2015\glcore\glcore.vcxproj x64 Debug "Debug DLL" Release "Release DLL"
REM call:build GL_platform_tools\build\vs2015\GL_platform_tools.vcxproj Win32 Debug "Debug DLL" Release "Release DLL"
call:build GL_platform_tools\build\vs2015\GL_platform_tools.vcxproj x64 Debug "Debug DLL" Release "Release DLL"

REM call:build GL_core_tools\build\vs2015\GL_core_tools.vcxproj Win32 Debug "Debug DLL" Release "Release DLL"
call:build GL_core_tools\build\vs2015\GL_core_tools.vcxproj x64 Debug "Debug DLL" Release "Release DLL"

REM call:build CUDA_core_tools\build\vs2015\CUDA_core_tools.vcxproj Win32 Debug Release
call:build CUDA_core_tools\build\vs2015\CUDA_core_tools.vcxproj x64 Debug Release

REM call:build CUDA_graphics_interop_tools\build\vs2015\CUDA_graphics_interop_tools.vcxproj Win32 Debug Release
call:build CUDA_graphics_interop_tools\build\vs2015\CUDA_graphics_interop_tools.vcxproj x64 Debug Release

REM call:build CUPTI_core_tools\build\vs2015\CUPTI_core_tools.vcxproj Win32 Debug Release
call:build CUPTI_core_tools\build\vs2015\CUPTI_core_tools.vcxproj x64 Debug Release

goto:eof


:build
	if "%~3"=="" goto:eof
	REM MSBuild /m /p:Platform=%2 /p:Configuration=%3 /t:Clean %1
	MSBuild /m /p:Platform=%2 /p:Configuration=%3 %1
	REM echo %1 %2 %3
	shift /3
	goto:build
	goto:eof
