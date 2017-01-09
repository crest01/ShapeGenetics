cmake_minimum_required(VERSION 2.8)

project(eval_gpuimage)

if (NOT DEFINED PROJECT_DIR)
	get_filename_component(PROJECT_DIR "../../../.." ABSOLUTE)
	MESSAGE( STATUS "Project dir:" ${PROJECT_DIR} )
endif()

if (NOT DEFINED Framework_LIBRARIES)
	message( SEND_ERROR "Framework required. This library can't be built on its own.")
endif()

set(SOURCE_DIR "${PROJECT_DIR}/gpuprocgenetics/eval/gpuimage")


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

file(GLOB glsl_SOURCES "${SOURCE_DIR}/shaders/*.glsl")

MESSAGE( STATUS "Framework include dirs: " ${Framework_INCLUDE_DIRS} )


include_directories( ${include_dirs_INTERNAL} ${gpuproccore_INCLUDE_DIRS} ${shared_INCLUDE_DIRS} ${Framework_INCLUDE_DIRS})


glsl_add_include_directories("${glsl_SOURCES}" "${SOURCE_DIR}/shaders")
add_glsl_sources(eval_gpuimage_shaders "${glsl_SOURCES}" )


add_library(eval_gpuimage STATIC ${h_SOURCES} ${cpp_SOURCES} )

set(eval_gpuimage_LIBRARIES eval_gpuimage eval_gpuimage_shaders ${Framework_LIBRARIES} ${shared_LIBRARIES} PARENT_SCOPE)
set(eval_gpuimage_INCLUDE_DIRS ${include_dirs_INTERNAL} PARENT_SCOPE)


	
