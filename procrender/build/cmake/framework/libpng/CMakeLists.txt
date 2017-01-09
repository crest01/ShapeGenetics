cmake_minimum_required(VERSION 2.8)

project(lpng)

set(SOURCE_DIR "${DEPENDENCIES_DIR}/libpng")
set(INCLUDE_DIR "${SOURCE_DIR}/include")

file(GLOB lpng_SOURCES "${SOURCE_DIR}/*.c")
file(GLOB lpng_HEADERS "${INCLUDE_DIR}/*.h")

include_directories(${ZLIB_INCLUDE_DIR} ${INCLUDE_DIR})
add_library(lpng STATIC ${lpng_SOURCES} ${lpng_HEADERS})

set(LPNG_LIBRARY lpng PARENT_SCOPE)
set(LPNG_INCLUDE_DIR "${INCLUDE_DIR}" PARENT_SCOPE)
