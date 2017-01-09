cmake_minimum_required(VERSION 2.8)

project(gpuproc_eval)

if (NOT DEFINED PROJECT_DIR)
	get_filename_component(PROJECT_DIR "../../../.." ABSOLUTE)
	MESSAGE( STATUS "Project dir:" ${PROJECT_DIR} )
endif()


set(eval_LIBRARIES_INTERNAL "")
set(eval_INCLUDE_DIRS_INTERNAL "")
if (EVAL_GPUVOLUME)
	add_subdirectory(${PROJECT_DIR}/gpuprocgenetics/build/cmake/eval/gpuvolume eval_gpuvolume)
	set(eval_LIBRARIES_INTERNAL ${eval_LIBRARIES_INTERNAL} ${eval_gpuvolume_LIBRARIES})
	set(eval_INCLUDE_DIRS_INTERNAL ${eval_INCLUDE_DIRS_INTERNAL} ${eval_gpuvolume_INCLUDE_DIRS})
	MESSAGE( STATUS "cmake/eval: gpuvol-libs:" ${eval_gpuvolume_LIBRARIES} )
endif()

if (EVAL_GPUIMAGE)
	add_subdirectory(${PROJECT_DIR}/gpuprocgenetics/build/cmake/eval/gpuimage eval_gpuimage)
	set(eval_LIBRARIES_INTERNAL ${eval_LIBRARIES_INTERNAL} ${eval_gpuimage_LIBRARIES})
	set(eval_INCLUDE_DIRS_INTERNAL ${eval_INCLUDE_DIRS_INTERNAL} ${eval_gpuimage_INCLUDE_DIRS})
endif()

if (EVAL_CPUVOLUME)
	add_subdirectory(${PROJECT_DIR}/gpuprocgenetics/build/cmake/eval/cpuvolume eval_cpuvolume)
	set(eval_LIBRARIES_INTERNAL ${eval_LIBRARIES_INTERNAL} ${eval_cpuvolume_LIBRARIES})
	set(eval_INCLUDE_DIRS_INTERNAL ${eval_INCLUDE_DIRS_INTERNAL} ${eval_cpuvolume_INCLUDE_DIRS})
endif()

set(eval_LIBRARIES ${eval_LIBRARIES_INTERNAL} PARENT_SCOPE)
set(eval_INCLUDE_DIRS ${eval_INCLUDE_DIRS_INTERNAL} PARENT_SCOPE)
