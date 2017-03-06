cmake_minimum_required(VERSION 2.8)

project(genetics_grammar)

if (NOT DEFINED PROJECT_DIR)
	get_filename_component(PROJECT_DIR "../../../.." ABSOLUTE)
	MESSAGE( STATUS "Project dir:" ${PROJECT_DIR} )
endif()


set(SOURCE_DIR "${PROJECT_DIR}/genetics/grammar")


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

include_directories( ${include_dirs_INTERNAL} ${proccore_INCLUDE_DIRS} ${shared_INCLUDE_DIRS})

MESSAGE( STATUS "Framework libs:" ${Framework_LIBRARIES} )

add_library(genetics_grammar STATIC ${h_SOURCES} ${cpp_SOURCES} )

set(grammar_LIBRARIES genetics_grammar ${procgenetics_shared_LIBRARIES} PARENT_SCOPE)
set(grammar_INCLUDE_DIRS ${include_dirs_INTERNAL} PARENT_SCOPE)


	
