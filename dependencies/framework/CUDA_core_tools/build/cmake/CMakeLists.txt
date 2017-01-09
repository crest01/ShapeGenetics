cmake_minimum_required(VERSION 2.8)

project(CUDA_core_tools)

find_package(CUDA REQUIRED)

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../include/")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../source/")

file(GLOB CUDA_core_tools_SOURCES ${SOURCE_DIR}/*.cpp)

include_directories(${CUDA_INCLUDE_DIRS} ${INCLUDE_DIR}CUDA ${CUDA_common_tools_INCLUDE_DIRS})

add_library(CUDA_core_tools STATIC ${CUDA_core_tools_SOURCES})

set(CUDA_core_tools_INCLUDE_DIRS ${INCLUDE_DIR} CACHE INTERNAL "CUDA core tools include directories")
set(CUDA_core_tools_LIBRARIES CUDA_core_tools PARENT_SCOPE)