cmake_minimum_required(VERSION 2.8)

project(CUDA_compiler_tools)

find_package(CUDA REQUIRED)

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../include/")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../source/")

file(GLOB CUDA_compiler_tools_SOURCES ${SOURCE_DIR}/*.cpp)

include_directories(${CUDA_INCLUDE_DIRS} ${CUDA_core_tools_INCLUDE_DIRS} ${CUDA_common_tools_INCLUDE_DIRS} ${INCLUDE_DIR})

add_library(CUDA_compiler_tools STATIC ${CUDA_compiler_tools_SOURCES})

set(CUDA_compiler_tools_INCLUDE_DIRS ${INCLUDE_DIR} CACHE INTERNAL "CUDA compiler tools include directories")

set(CUDA_compiler_tools_LIBRARIES CUDA_compiler_tools PARENT_SCOPE)
