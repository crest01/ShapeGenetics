cmake_minimum_required(VERSION 2.8)

project(procrender)

include_directories(${Framework_INCLUDE_DIRS} ${proccore_INCLUDE_DIRS} ${genetics_INCLUDE_DIRS})

file(GLOB h_SOURCES "${SOURCE_DIR}/*.h")
file(GLOB cpp_SOURCES "${SOURCE_DIR}/*.cpp")
file(GLOB glsl_SOURCES "${SOURCE_DIR}/shaders/*.glsl")

glsl_add_include_directories("${glsl_SOURCES}" "${Framework_INCLUDE_DIRS}")
add_glsl_sources(${PROJECT_NAME}_shaders "${glsl_SOURCES}")

add_executable(${PROJECT_NAME} ${h_SOURCES} ${cpp_SOURCES} ${glsl_SOURCES})

message(STATUS "cmake/src: Framework libs: ${Framework_LIBRARIES}")

message(STATUS "cmake/src: genetic libs: ${genetics_LIBRARIES}")

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_shaders ${Framework_LIBRARIES} ${genetics_LIBRARIES} ${proccore_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${eval_LIBRARIES} nvcuda)
