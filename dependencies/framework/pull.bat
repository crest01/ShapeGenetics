@echo off

call:pull https://khauben@bitbucket.org/michael_kenzel/Win32_core_tools Win32_core_tools

call:pull https://khauben@bitbucket.org/michael_kenzel/GL_platform_tools GL_platform_tools
call:pull https://khauben@bitbucket.org/michael_kenzel/GL_core_tools GL_core_tools
call:pull https://khauben@bitbucket.org/michael_kenzel/GLSL_build_tools GLSL_build_tools

call:pull https://khauben@bitbucket.org/khauben/CUDA_core_tools CUDA_core_tools
call:pull https://khauben@bitbucket.org/khauben/CUDA_common_tools CUDA_common_tools
call:pull https://khauben@bitbucket.org/khauben/cuda_compiler_tools CUDA_compiler_tools

goto:eof


:pull
	if not exist %2 (
		hg clone %1 %2
	)
	cd %2
	hg pull
	cd ..
	goto:eof
