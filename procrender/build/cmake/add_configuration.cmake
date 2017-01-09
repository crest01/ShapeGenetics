
macro(add_configuration name basecfg)
  string(TOUPPER ${ARGV0} CFG)
  string(TOUPPER ${ARGV1} BASECFG)
  
  set(CMAKE_C_FLAGS_${CFG}
    ${CMAKE_C_FLAGS_${BASECFG}}                 CACHE STRING "Flags used by the compiler during ${ARGV0} builds")
  set(CMAKE_CXX_FLAGS_${CFG}
    ${CMAKE_CXX_FLAGS_${BASECFG}}               CACHE STRING "Flags used by the compiler during ${ARGV0} builds")
  set(CMAKE_EXE_LINKER_FLAGS_${CFG}
    ${CMAKE_EXE_LINKER_FLAGS_${BASECFG}}        CACHE STRING "Flags used by the linker for executables during ${ARGV0} builds")
  set(CMAKE_SHARED_LINKER_FLAGS_${CFG}
    ${CMAKE_SHARED_LINKER_FLAGS_${BASECFG}}     CACHE STRING "Flags used by the linker for shared libraries during ${ARGV0} builds")
  set(CMAKE_MODULE_LINKER_FLAGS_${CFG}
    ${CMAKE_MODULE_LINKER_FLAGS_${BASECFG}}     CACHE STRING "Flags used by the linker for loadable modules during ${ARGV0} builds")

  mark_as_advanced(
      CMAKE_C_FLAGS_${CFG}
      CMAKE_CXX_FLAGS_${CFG}
      CMAKE_EXE_LINKER_FLAGS_${CFG}
      CMAKE_SHARED_LINKER_FLAGS_${CFG}
      CMAKE_MODULE_LINKER_FLAGS_${CFG}
  )

  if(CMAKE_CONFIGURATION_TYPES)
      list(APPEND CMAKE_CONFIGURATION_TYPES ${ARGV0})
      list(REMOVE_DUPLICATES CMAKE_CONFIGURATION_TYPES)
      set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}"
          CACHE STRING "Semicolon separated list of supported configuration types [Debug|Release|MinSizeRel|RelWithDebInfo|${ARGV0}]"
          FORCE)
  endif()
endmacro()
