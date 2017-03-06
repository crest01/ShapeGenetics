cmake_minimum_required(VERSION 2.8)

project(genetics_algo)

if (NOT DEFINED PROJECT_DIR)
	get_filename_component(PROJECT_DIR "../../../.." ABSOLUTE)
	MESSAGE( STATUS "Project dir:" ${PROJECT_DIR} )
endif()

if (NOT DEFINED proccore_INCLUDE_DIRS)
	message( SEND_ERROR "proccore required. This library can't be built on its own.")
endif()


set(SOURCE_DIR "${PROJECT_DIR}/genetics/algo")


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

include_directories(${include_dirs_INTERNAL} ${proccore_INCLUDE_DIRS} ${metropolis_INCLUDE_DIRS} ${shared_INCLUDE_DIRS} ${genetics_INCLUDE_DIRS} ${grammar_INCLUDE_DIRS} ${eval_INCLUDE_DIRS} ${Framework_INCLUDE_DIRS})



add_library(genetics_algo STATIC ${h_SOURCES} ${cpp_SOURCES} )

set(algo_LIBRARIES genetics_algo ${proccore_LIBRARIES} ${metropolis_LIBRARIES} ${shared_LIBRARIES} ${genetics_LIBRARIES} ${grammar_LIBRARIES} ${eval_LIBRARIES} PARENT_SCOPE)
set(algo_INCLUDE_DIRS ${include_dirs_INTERNAL} PARENT_SCOPE)

MESSAGE( STATUS "cmake/algo eval libraries: " ${eval_LIBRARIES} )

	
