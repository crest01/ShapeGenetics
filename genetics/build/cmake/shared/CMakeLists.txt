cmake_minimum_required(VERSION 2.8)

project(genetics_shared)

if (NOT DEFINED PROJECT_DIR)
	get_filename_component(PROJECT_DIR "../../../.." ABSOLUTE)
	MESSAGE( STATUS "Project dir:" ${PROJECT_DIR} )
endif()

if (NOT DEFINED proccore_INCLUDE_DIRS)
	message( SEND_ERROR "proccore required. This library can't be built on its own.")
endif()


set(SOURCE_DIR "${PROJECT_DIR}/genetics/shared")


if (UNIX)
	set(CMAKE_C_FLAGS "-std=c90")
	set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

 
set(include_dirs_INTERNAL 
	${SOURCE_DIR}/configfile
	${SOURCE_DIR}/geometry
	${SOURCE_DIR}/interfaces
	${SOURCE_DIR}/symbols
	${SOURCE_DIR}/utils
	${SOURCE_DIR}
	)

file(GLOB h_SOURCES 
		"${SOURCE_DIR}/configfile/*.h"
		"${SOURCE_DIR}/geometry/*.h"
		"${SOURCE_DIR}/interfaces/*.h"
		"${SOURCE_DIR}/symbols/*.h"
		"${SOURCE_DIR}/utils/*.h"
		"${SOURCE_DIR}/*.h"
)

file(GLOB cpp_SOURCES 
		"${SOURCE_DIR}/configfile/*.cpp"
		"${SOURCE_DIR}/geometry/*.cpp"
		"${SOURCE_DIR}/interfaces/*.cpp"
		"${SOURCE_DIR}/symbols/*.cpp"
		"${SOURCE_DIR}/utils/*.cpp"
		"${SOURCE_DIR}/*.cpp"
)

include_directories( ${include_dirs_INTERNAL} ${proccore_INCLUDE_DIRS} ${Framework_INCLUDE_DIRS})

add_library(genetics_shared STATIC ${h_SOURCES} ${cpp_SOURCES} )

set(shared_LIBRARIES genetics_shared ${proccore_LIBRARIES} PARENT_SCOPE)
set(shared_INCLUDE_DIRS ${include_dirs_INTERNAL} PARENT_SCOPE)


	
