cmake_minimum_required(VERSION 2.8)

find_package(CUDA REQUIRED)

project(CUDA_graphics_interop_tools)

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../include/")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../source/")


file(GLOB CUDA_graphics_interop_tools_SOURCES ${SOURCE_DIR}/*.cpp)

include_directories(${GL_platform_tools_INCLUDE_DIRS} ${CUDA_INCLUDE_DIRS} ${CUDA_core_tools_INCLUDE_DIRS} ${INCLUDE_DIR}CUDA)
if (WIN32)
include_directories(${Win32_core_tools_INCLUDE_DIRS})
endif ()

add_library(CUDA_graphics_interop_tools STATIC ${CUDA_graphics_interop_tools_SOURCES})

set(CUDA_graphics_interop_tools_INCLUDE_DIRS ${INCLUDE_DIR} CACHE INTERNAL "CUDA graphics interop tools include directories")

set(CUDA_graphics_interop_tools_LIBRARIES CUDA_graphics_interop_tools PARENT_SCOPE) 