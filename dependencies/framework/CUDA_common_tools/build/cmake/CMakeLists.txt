cmake_minimum_required(VERSION 2.8)

project(CUDA_common_tools)

find_package(CUDA REQUIRED)

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../include/" ${CUDA_INCLUDE_DIRS})
#set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../source/")


#file(GLOB CUDA_common_tools_SOURCES ${SOURCE_DIR}/*.cpp)

#include_directories(${CUDA_INCLUDE_DIRS}/ ${INCLUDE_DIR}CUDA)

#add_library(CUDA_common_tools STATIC ${CUDA_common_tools_SOURCES})

set(CUDA_common_tools_INCLUDE_DIRS ${INCLUDE_DIR} CACHE INTERNAL "CUDA common tools include directories")
