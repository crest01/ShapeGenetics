cmake_minimum_required(VERSION 2.8)

project(proccore)

if (NOT DEFINED PROJECT_DIR)
	get_filename_component(PROJECT_DIR "../../.." ABSOLUTE)
endif()

set(SOURCE_DIR "${PROJECT_DIR}/proccore/src")
set(DEPENDENCIES_DIR "${PROJECT_DIR}/proccore/build/dependencies")



if (UNIX)
	set(CMAKE_C_FLAGS "-std=c90")
	set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

 

set(include_dirs_INTERNAL
	${SOURCE_DIR}
	${SOURCE_DIR}/shapes
	${SOURCE_DIR}/CPU
	${SOURCE_DIR}/CPUDynamic
	${SOURCE_DIR}/modifiers
	${SOURCE_DIR}/operators
	${SOURCE_DIR}/parameters
	${SOURCE_DIR}/scope
	${DEPENDENCIES_DIR}/utils)

file(GLOB h_SOURCES 
		"${SOURCE_DIR}/*.h"
		"${SOURCE_DIR}/CPU/*.h"
		"${SOURCE_DIR}/CPUDynamic/*.h"
		"${SOURCE_DIR}/modifiers/*.h"
		"${SOURCE_DIR}/operators/*.h"
		"${SOURCE_DIR}/parameters/*.h"
		"${SOURCE_DIR}/scope/*.h"
		"${SOURCE_DIR}/shapes/*.h"
)

file(GLOB cpp_SOURCES 
		"${SOURCE_DIR}/*.cpp"
		"${SOURCE_DIR}/CPU/*.cpp"
		"${SOURCE_DIR}/CPUDynamic/*.cpp"
		"${SOURCE_DIR}/modifiers/*.cpp"
		"${SOURCE_DIR}/operators/*.cpp"
		"${SOURCE_DIR}/parameters/*.cpp"
		"${SOURCE_DIR}/scope/*.cpp"
		"${SOURCE_DIR}/shapes/*.cpp"
)

#note: no CUDA as of yet
file(GLOB cuh_SOURCES "${SOURCE_DIR}/*.cuh")
file(GLOB cu_SOURCES "${SOURCE_DIR}/*.cu")

find_package(Threads REQUIRED)

include_directories( "${include_dirs_INTERNAL}" )

add_library(proccore STATIC ${h_SOURCES} ${cpp_SOURCES} ${glsl_SOURCES} ${cu_SOURCES} ${cuh_SOURCES})

set(proccore_LIBRARIES proccore ${CMAKE_THREAD_LIBS_INIT} PARENT_SCOPE)
set(proccore_INCLUDE_DIRS ${include_dirs_INTERNAL} PARENT_SCOPE)
	
