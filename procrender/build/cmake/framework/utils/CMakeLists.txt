cmake_minimum_required(VERSION 2.8)

project(utils)

set(SOURCE_DIR "${DEPENDENCIES_DIR}/utils")

file(GLOB utils_SOURCES "${SOURCE_DIR}/*.h" "${SOURCE_DIR}/*.cpp")

set(utils_INCLUDE_DIR "${SOURCE_DIR}" PARENT_SCOPE)
