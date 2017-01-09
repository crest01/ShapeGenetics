cmake_minimum_required(VERSION 2.8)

project(gpuprocgenetics_metropolis)

if (NOT DEFINED PROJECT_DIR)
	get_filename_component(PROJECT_DIR "../../../.." ABSOLUTE)
	MESSAGE( STATUS "Project dir:" ${PROJECT_DIR} )
endif()

if (NOT DEFINED gpuproccore_INCLUDE_DIRS)
	message( SEND_ERROR "gpuproccore required. This library can't be built on its own.")
endif()


set(SOURCE_DIR "${PROJECT_DIR}/gpuprocgenetics/metropolis")


if (UNIX)
	set(CMAKE_C_FLAGS "-std=c90")
	set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

 
set(include_dirs_INTERNAL 
	${SOURCE_DIR}
	)

file(GLOB h_SOURCES 
		"${SOURCE_DIR}/*.h"
)

file(GLOB cpp_SOURCES 
		"${SOURCE_DIR}/*.cpp"
)

include_directories(${include_dirs_INTERNAL} ${gpuproccore_INCLUDE_DIRS} ${shared_INCLUDE_DIRS} ${genetics_INCLUDE_DIRS} ${grammar_INCLUDE_DIRS} ${eval_INCLUDE_DIRS})

MESSAGE( STATUS "eval include dirs: " ${eval_INCLUDE_DIRS} )


add_library(gpuprocgenetics_metropolis STATIC ${h_SOURCES} ${cpp_SOURCES} )

set(metropolis_LIBRARIES gpuprocgenetics_metropolis ${gpuproccore_LIBRARIES} ${shared_LIBRARIES} ${genetics_LIBRARIES} ${grammar_LIBRARIES} ${eval_LIBRARIES} PARENT_SCOPE)
set(metropolis_INCLUDE_DIRS ${include_dirs_INTERNAL} PARENT_SCOPE)


	
