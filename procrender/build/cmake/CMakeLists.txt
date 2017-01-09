cmake_minimum_required(VERSION 2.8)

include("add_configuration.cmake")
include("add_subdirectory_if_exists.cmake")

project(gpuprocrender)

add_definitions(-DLEFTHANDED)


set(CMAKE_BUILD_TYPE DEBUG) 
#set(CMAKE_BUILD_TYPE RELEASE) 

if (WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
	add_definitions(-DGLCORE_STATIC)
else ()
	set(CMAKE_C_FLAGS "-std=c90")
	set(CMAKE_CXX_FLAGS "-std=c++14")
	set(CMAKE_EXE_LINKER_FLAGS "-lX11 -lcuda")
endif ()
endif()


#####################################################################################
#set up the paths. this asumes i'm inside 'build/cmake/_something_/'
#
# PROJECT_DIR: folder containing both gpuproccore and gpuprocrender
# SOURCE_DIR: folder containing the source-code for gpuprocrender
# DEPENDENCIES_DIR: folder containing the dependencies (framework) for gpuprocrender
##################################################################################### 
get_filename_component(PROJECT_DIR "../../.." ABSOLUTE)
get_filename_component(SOURCE_DIR "../../src" ABSOLUTE)
get_filename_component(DEPENDENCIES_DIR "../../dependencies" ABSOLUTE)

add_subdirectory(framework)

add_subdirectory(${PROJECT_DIR}/proccore/build/cmake proccore)
add_subdirectory(${PROJECT_DIR}/genetics/build/cmake genetics)

add_subdirectory(src)

