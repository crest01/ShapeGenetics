@echo off

call:update Win32_core_tools
call:update GL_platform_tools
call:update GL_core_tools
call:update GLSL_build_tools
call:update CUDA_core_tools
call:update CUDA_graphics_interop_tools
call:update CUPTI_core_tools
call:update CUDA_build_tools
call:update COFF_tools


call:instantiate GL_platform_tools\build\vs2015\dependencies.props.template
call:instantiate GL_core_tools\build\vs2015\dependencies.props.template

call:instantiate CUDA_core_tools\build\vs2015\cudatoolkit.props.template
call:instantiate CUDA_graphics_interop_tools\build\vs2015\dependencies.props.template
call:instantiate CUPTI_core_tools\build\vs2015\cupti.props.template
call:instantiate CUPTI_core_tools\build\vs2015\dependencies.props.template


goto:eof


:update
	cd "%~1"
	hg update %2 %3
	cd ..
	goto:eof

:instantiate
	echo instantiating %1
	copy "%~1" "%~p1%~n1"
	goto:eof
