cmake_minimum_required(VERSION 2.8)

project(CUPTI_core_tools)

find_package(CUDA REQUIRED)

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../include/")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../source/")


file(GLOB CUPTI_core_tools_SOURCES ${SOURCE_DIR}/*.cpp)

include_directories(${CUDA_INCLUDE_DIRS} ${INCLUDE_DIR}CUPTI)
include_directories("${CUDA_TOOLKIT_ROOT_DIR}/extras/CUPTI/include")

add_library(CUPTI_core_tools STATIC ${CUPTI_core_tools_SOURCES})

set(CUPTI_core_tools_INCLUDE_DIRS ${INCLUDE_DIR} CACHE INTERNAL "CUPTI core tools include directories")

set(CUPTI_core_tools_LIBRARIES CUPTI_core_tools PARENT_SCOPE)