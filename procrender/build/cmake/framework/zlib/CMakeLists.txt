cmake_minimum_required(VERSION 2.8)
project(zlib)
 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_DIR "${DEPENDENCIES_DIR}/zlib")
 
set(INCLUDE_DIR "${SOURCE_DIR}/include")
 
message(STATUS "include dir: ${INCLUDE_DIR}")
 
file(GLOB zlib_SOURCES ${SOURCE_DIR}/*.c)
file(GLOB zlib_HEADER ${INCLUDE_DIR}/*.h)


include_directories(${INCLUDE_DIR})

add_library(zlib STATIC ${zlib_SOURCES} ${zlib_HEADER})

set(ZLIB_LIBRARY zlib PARENT_SCOPE)
set(ZLIB_INCLUDE_DIR "${INCLUDE_DIR}" PARENT_SCOPE)