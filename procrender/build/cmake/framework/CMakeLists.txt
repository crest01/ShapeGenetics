cmake_minimum_required(VERSION 2.8)

project(framework)

find_package(OpenGL REQUIRED)
find_package(CUDA REQUIRED)
set(CMAKE_DEBUG_POSTFIX "D")

if (WIN32)
	add_subdirectory(${DEPENDENCIES_DIR}/framework/Win32_core_tools/build/cmake Win32_core_tools)
endif()

MESSAGE( STATUS "DEPENDENCIES_DIR: " ${DEPENDENCIES_DIR} )

include("${DEPENDENCIES_DIR}/framework/GLSL_build_tools/build/cmake/add_glsl_sources.cmake")

add_subdirectory(${DEPENDENCIES_DIR}/framework/CUDA_common_tools/build/cmake CUDA_common_tools)
add_subdirectory(${DEPENDENCIES_DIR}/framework/CUDA_core_tools/build/cmake CUDA_core_tools)
add_subdirectory(${DEPENDENCIES_DIR}/framework/CUDA_compiler_tools/build/cmake CUDA_compiler_tools)
add_subdirectory(${DEPENDENCIES_DIR}/framework/GL_platform_tools/build/cmake GL_platform_tools)
add_subdirectory(${DEPENDENCIES_DIR}/framework/GL_core_tools/build/cmake GL_core_tools)
#add_subdirectory(${DEPENDENCIES_DIR}/framework/CUDA_graphics_interop_tools/build/cmake CUDA_graphics_interop_tools)
#add_subdirectory(${DEPENDENCIES_DIR}/framework/CUPTI_core_tools/build/cmake CUPTI_core_tools)

if (WIN32)
	add_subdirectory(zlib zlib)
	add_subdirectory(libpng libpng)
else()
	find_package(ZLIB REQUIRED)
	find_package(PNG REQUIRED)
	
endif()

add_subdirectory_if_exists(utils)

set(Framework_INCLUDE_DIRS_internal 
	${CUDA_common_tools_INCLUDE_DIRS}
	${CUDA_core_tools_INCLUDE_DIRS} 
	${GL_core_tools_INCLUDE_DIRS}
#	${CUDA_graphics_interop_tools_INCLUDE_DIRS}
#	${CUPTI_core_tools_INCLUDE_DIRS}
	${CUDA_compiler_tools_INCLUDE_DIRS}
	${GL_platform_tools_INCLUDE_DIRS})

#Win32_core_tools should be here somewhere

if (WIN32)
	set(Framework_INCLUDE_DIRS 
				${Framework_INCLUDE_DIRS_internal} ${ZLIB_INCLUDE_DIR} ${LPNG_INCLUDE_DIR} ${Win32_core_tools_INCLUDE_DIR} PARENT_SCOPE)
	set(Framework_LIBRARIES  
						${LPNG_LIBRARY} 
						${ZLIB_LIBRARY} 
						${CMAKE_DL_LIBS} 
						${GL_platform_tools_LIBRARIES}
						${GL_core_tools_LIBRARIES}
						${CUDA_common_tools_LIBRARIES}
						${CUDA_core_tools_LIBRARIES}
						${CUDA_compiler_tools_LIBRARIES}
						${CUDA_LIBRARIES}
						

GL_core_tools
						Win32_core_tools
						PARENT_SCOPE)
	MESSAGE(STATUS "cmake/framework: CUDA compiler tools libs: " ${CUDA_compiler_tools_LIBRARIES})
	MESSAGE(STATUS "cmake/framework: CUDA core tools libs: " ${CUDA_core_tools_LIBRARIES})
	MESSAGE(STATUS "cmake/framework: CUDA Libraries: " ${CUDA_LIBRARIES})
else ()
	set(Framework_INCLUDE_DIRS ${Framework_INCLUDE_DIRS_internal} PARENT_SCOPE)
	set(Framework_LIBRARIES 
		${PNG_LIBRARIES} 
		${ZLIB_LIBRARIES} 
		${CMAKE_DL_LIBS} 		
		${GL_platform_tools_LIBRARIES}
		${GL_core_tools_LIBRARIES}
		${CUDA_common_tools_LIBRARIES}
		${CUDA_core_tools_LIBRARIES}
		${CUDA_compiler_tools_LIBRARIES}
		${CUDA_LIBRARIES}
		PARENT_SCOPE)
	MESSAGE(STATUS "cmake/framework: CUDA compiler tools libs: " ${CUDA_compiler_tools_LIBRARIES})
	MESSAGE(STATUS "cmake/framework: CUDA core tools libs: " ${CUDA_core_tools_LIBRARIES})
endif ()
