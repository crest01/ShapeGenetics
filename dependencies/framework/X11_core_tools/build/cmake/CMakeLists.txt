cmake_minimum_required(VERSION 2.8)

project(X11_core_tools)

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../include/")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../source/")

file(GLOB X11_core_tools_SOURCES "${SOURCE_DIR}/*.cpp")

include_directories(${INCLUDE_DIR})

add_library(X11_core_tools STATIC ${X11_core_tools_SOURCES})

set(X11_core_tools_LIBRARY X11_core_tools PARENT_SCOPE)
set(X11_core_tools_INCLUDE_DIR "${INCLUDE_DIR}" PARENT_SCOPE)
