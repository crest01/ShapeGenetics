cmake_minimum_required(VERSION 2.8)

project(gpuprocgenetics_algo)

if (NOT DEFINED PROJECT_DIR)
	get_filename_component(PROJECT_DIR "../../../.." ABSOLUTE)
	MESSAGE( STATUS "Project dir:" ${PROJECT_DIR} )
endif()

if (NOT DEFINED gpuproccore_INCLUDE_DIRS)
	message( SEND_ERROR "gpuproccore required. This library can't be built on its own.")
endif()


set(SOURCE_DIR "${PROJECT_DIR}/gpuprocgenetics/algo")


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

MESSAGE(STATUS "cmake/algo: eval include dirs: " ${eval_INCLUDE_DIRS})

include_directories(${include_dirs_INTERNAL} ${gpuproccore_INCLUDE_DIRS} ${metropolis_INCLUDE_DIRS} ${shared_INCLUDE_DIRS} ${genetics_INCLUDE_DIRS} ${grammar_INCLUDE_DIRS} ${eval_INCLUDE_DIRS} ${Framework_INCLUDE_DIRS})



add_library(gpuprocgenetics_algo STATIC ${h_SOURCES} ${cpp_SOURCES} )

set(algo_LIBRARIES gpuprocgenetics_algo ${gpuproccore_LIBRARIES} ${metropolis_LIBRARIES} ${shared_LIBRARIES} ${genetics_LIBRARIES} ${grammar_LIBRARIES} ${eval_LIBRARIES} PARENT_SCOPE)
set(algo_INCLUDE_DIRS ${include_dirs_INTERNAL} PARENT_SCOPE)

MESSAGE( STATUS "cmake/algo eval libraries: " ${eval_LIBRARIES} )

	
