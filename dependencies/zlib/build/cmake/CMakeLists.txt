cmake_minimum_required (VERSION 2.8)

project(zlib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../")

set(INCLUDE_DIR "${SOURCE_DIR}/include/")
set(SOURCE_DIR "${SOURCE_DIR}/")

file(GLOB SOURCES ${SOURCE_DIR}/*.c)
file(GLOB HEADER ${INCLUDE_DIR}/*.h)


include_directories(
	${INCLUDE_DIR}
)

add_library(zlib STATIC ${SOURCES})

