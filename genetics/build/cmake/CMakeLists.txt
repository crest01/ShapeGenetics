cmake_minimum_required(VERSION 2.8)

project(genetics)

if (NOT DEFINED PROJECT_DIR)
	get_filename_component(PROJECT_DIR "../../.." ABSOLUTE)
	add_subdirectory(${PROJECT_DIR}/genetics/build/cmake genetics)
endif()

add_subdirectory(${PROJECT_DIR}/genetics/build/cmake/shared genetics_shared)

option(EVAL_GPUVOLUME "Build with volumetric gpu evaluation (CUDA)" TRUE)
option(EVAL_GPUIMAGE "Build with volumetric cpu evaluation" TRUE)
option(EVAL_CPUVOLUME "Build with image based evaluation (OpenGL compute shaders)" TRUE)

add_subdirectory(${PROJECT_DIR}/genetics/build/cmake/eval genetics_eval)

if (EVAL_GPUVOLUME)
	add_definitions(-DEVAL_GPUVOLUME)
	MESSAGE(STATUS "adding GPUVolume")
endif()

if (EVAL_GPUIMAGE)
	add_definitions(-DEVAL_GPUIMAGE)
	MESSAGE(STATUS "adding GPUImage")
endif()

if (EVAL_CPUVOLUME)
	add_definitions(-DEVAL_CPUVOLUME)
	MESSAGE(STATUS "adding CPUVolume")
endif()

add_subdirectory(${PROJECT_DIR}/genetics/build/cmake/grammar genetics_grammar)
add_subdirectory(${PROJECT_DIR}/genetics/build/cmake/genetics genetics_genetics)

add_subdirectory(${PROJECT_DIR}/genetics/build/cmake/metropolis genetics_metropolis)
add_subdirectory(${PROJECT_DIR}/genetics/build/cmake/algo genetics_algo)

set(genetics_LIBRARIES ${metropolis_LIBRARIES} ${genetics_LIBRARIES} ${grammar_LIBRARIES} ${algo_LIBRARIES} ${shared_LIBRARIES} ${eval_LIBRARIES} PARENT_SCOPE)
set(genetics_INCLUDE_DIRS ${shared_INCLUDE_DIRS} ${metropolis_INCLUDE_DIRS} ${genetics_INCLUDE_DIRS} ${grammar_INCLUDE_DIRS} ${eval_INCLUDE_DIRS} ${algo_INCLUDE_DIRS} PARENT_SCOPE)


	
