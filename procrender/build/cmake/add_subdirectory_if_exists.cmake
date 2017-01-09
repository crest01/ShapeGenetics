cmake_minimum_required(VERSION 2.8)

macro (ADD_SUBDIRECTORY_IF_EXISTS dir)
	if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${dir}")
		add_subdirectory(${dir})
	endif (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${dir}")
endmacro (ADD_SUBDIRECTORY_IF_EXISTS)
