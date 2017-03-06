cmake_minimum_required(VERSION 2.8)

project(eval_gpuvolume)

find_package(CUDA)

if (NOT DEFINED PROJECT_DIR)
	get_filename_component(PROJECT_DIR "../../../.." ABSOLUTE)
	MESSAGE( STATUS "Project dir:" ${PROJECT_DIR} )
endif()

if (NOT DEFINED Framework_LIBRARIES)
	message( SEND_ERROR "Framework required. This library can't be built on its own.")
endif()


set(SOURCE_DIR "${PROJECT_DIR}/genetics/eval/gpuvolume")


if (UNIX)
	set(CMAKE_C_FLAGS "-std=c90")
	set(CMAKE_CXX_FLAGS "-std=c++14 -D_FORCE_INLINES")
endif()

#option(CUDA_BUILD_CC20 "Build with compute capability 2.0 support" FALSE)
#option(CUDA_BUILD_CC21 "Build with compute capability 2.1 support" FALSE)
#option(CUDA_BUILD_CC30 "Build with compute capability 3.0 support" FALSE)
#option(CUDA_BUILD_CC35 "Build with compute capability 3.5 support" FALSE)
#option(CUDA_BUILD_CC50 "Build with compute capability 5.0 support" TRUE)
#option(CUDA_BUILD_INFO "Build with kernel statistics and line numbers" FALSE)
#option(CUDA_BUILD_DEBUG "Build with kernel debug" FALSE)

#set(CUDA_NVCC_FLAGS "-use_fast_math")
#if(CUDA_BUILD_CC20)
#	set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS};-gencode=arch=compute_20,code=sm_20;")
#endif()
#if(CUDA_BUILD_CC21)
#	set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS};-gencode=arch=compute_20,code=sm_21;")
#endif()
#if(CUDA_BUILD_CC30)
#	set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS};-gencode=arch=compute_30,code=sm_30;")
#endif()
#if(CUDA_BUILD_CC35 OR CUDA_BUILD_CC35_NODYN)
#	set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS};-gencode=arch=compute_35,code=sm_35;-maxrregcount=63;")
#endif()
#if(CUDA_BUILD_CC50)
#	set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS};-gencode=arch=compute_50,code=sm_50;-maxrregcount=63;")
#endif()
#if(CUDA_BUILD_INFO)
#	set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS};-keep;--ptxas-options=-v;-lineinfo")
#endif()
#if(CUDA_BUILD_DEBUG)
#	set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS};-G -O0 -g")
#else()
#	set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS};-O3")
#endif()


# NVRTC
find_path(CUDA_NVRTC_INCLUDE_DIR NAMES nvrtc.h PATHS "${CUDA_TOOLKIT_ROOT_DIR}/include/")
set(CUDA_NVRTC_INCLUDE_DIRS ${CUDA_NVRTC_INCLUDE_DIR} CACHE INTERNAL PATH)

if (WIN32)
	 find_library(CUDA_NVRTC_LIBRARY NAMES nvrtc.lib PATHS "${CUDA_TOOLKIT_ROOT_DIR}/lib/x64/")
else ()
	 find_library(CUDA_NVRTC_LIBRARY NAMES libnvrtc.so PATHS "${CUDA_TOOLKIT_ROOT_DIR}/lib64/")
endif ()

set(CUDA_NVRTC_LIBRARIES ${CUDA_NVRTC_LIBRARY})


set(include_dirs_INTERNAL 
	${SOURCE_DIR}
	)

file(GLOB h_SOURCES 
		"${SOURCE_DIR}/*.h"
)

file(GLOB cpp_SOURCES 
		"${SOURCE_DIR}/*.cpp"
)

#file(GLOB cuh_SOURCES "${SOURCE_DIR}/*.cuh")
#file(GLOB cu_SOURCES "${SOURCE_DIR}/*.cu")

include_directories( ${include_dirs_INTERNAL} ${proccore_INCLUDE_DIRS} ${shared_INCLUDE_DIRS} ${Framework_INCLUDE_DIRS})


add_library(eval_gpuvolume STATIC ${h_SOURCES} ${cpp_SOURCES} )
#CUDA_ADD_LIBRARY(eval_gpuvolume_cuda STATIC ${cu_SOURCES} ${cuh_SOURCES})

#set(eval_gpuvolume_LIBRARIES eval_gpuvolume eval_gpuvolume_cuda ${shared_LIBRARIES} PARENT_SCOPE)
set(eval_gpuvolume_LIBRARIES eval_gpuvolume ${shared_LIBRARIES} ${Framework_LIBRARIES} ${CUDA_NVRTC_LIBRARIES} PARENT_SCOPE)
set(eval_gpuvolume_INCLUDE_DIRS ${include_dirs_INTERNAL} PARENT_SCOPE)


