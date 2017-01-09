#/bin/bash

function clone {
if [ ! -d $2 ]; then
	hg clone $1 $2;
fi

pushd ./$2
hg pull
popd
}

clone ssh://hg@bitbucket.org/michael_kenzel/GL_platform_tools GL_platform_tools
clone ssh://hg@bitbucket.org/michael_kenzel/GL_core_tools GL_core_tools
clone ssh://hg@bitbucket.org/michael_kenzel/GLSL_build_tools GLSL_build_tools

clone ssh://hg@bitbucket.org/michael_kenzel/CUDA_core_tools CUDA_core_tools
clone ssh://hg@bitbucket.org/michael_kenzel/CUDA_graphics_interop_tools CUDA_graphics_interop_tools
clone ssh://hg@bitbucket.org/michael_kenzel/CUPTI_core_tools CUPTI_core_tools
clone ssh://hg@bitbucket.org/michael_kenzel/CUDA_build_tools CUDA_build_tools

clone ssh://hg@bitbucket.org/michael_kenzel/COFF_tools COFF_tools

exit


