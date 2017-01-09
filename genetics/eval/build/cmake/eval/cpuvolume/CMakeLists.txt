cmake_minimum_required(VERSION 2.8)

project(eval_cpuvolume)

if (NOT DEFINED PROJECT_DIR)
	get_filename_component(PROJECT_DIR "../../../.." ABSOLUTE)
	MESSAGE( STATUS "Project dir:" ${PROJECT_DIR} )
endif()

set(SOURCE_DIR "${PROJECT_DIR}/gpuprocgenetics/eval/cpuvolume")


if (UNIX)
	set(CMAKE_C_FLAGS "-std=c90")
	set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

 
set(include_dirs_INTERNAL 
	${SOURCE_DIR}
	${PROJECT_DIR}/gpuproccore_ga/build/dependencies/utils
	${PROJECT_DIR}/gpuproccore_ga/src/CPU
)

file(GLOB h_SOURCES 
		"${SOURCE_DIR}/*.h"
)

file(GLOB cpp_SOURCES 
		"${SOURCE_DIR}/*.cpp"
)

include_directories( ${include_dirs_INTERNAL} ${gpuproccore_INCLUDE_DIRS} ${shared_INCLUDE_DIRS})

MESSAGE( STATUS "Framework libs:" ${Framework_LIBRARIES} )

add_library(eval_cpuvolume STATIC ${h_SOURCES} ${cpp_SOURCES} )

set(eval_cpuvolume_LIBRARIES eval_cpuvolume ${shared_LIBRARIES} PARENT_SCOPE)
set(eval_cpuvolume_INCLUDE_DIRS ${include_dirs_INTERNAL} PARENT_SCOPE)


	
