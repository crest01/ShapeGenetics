cmake_minimum_required (VERSION 2.8)

project(png)

set(ZLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../zlib/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../include/")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../")

file(GLOB SOURCES ${SOURCE_DIR}/*.c)

include_directories(
	${ZLIB_DIR}
	${INCLUDE_DIR}
)

add_library(png STATIC ${SOURCES})

