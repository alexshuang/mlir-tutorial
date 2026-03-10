cmake_minimum_required(VERSION 3.18)

include(ExternalProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(mlir-tutorial CXX C)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(BUILD_WITH_CCACHE ON)

if(BUILD_WITH_CCACHE)
    find_program(CCACHE_BIN ccache)
    if(CCACHE_BIN)
        set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_BIN}
            CACHE STRING "c compiler launcher")
        set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_BIN}
            CACHE STRING "cxx compiler launcher")
    else()
        message(
            STATUS
                "Could not found ccache. Consider installing ccache to speed up compilation."
        )
    endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

find_package(MLIR REQUIRED CONFIG PATHS ${MLIR_DIR})

list(APPEND CMAKE_MODULE_PATH ${MLIR_CMAKE_DIR})
list(APPEND CMAKE_MODULE_PATH ${LLVM_CMAKE_DIR})

include(TableGen)
include(AddLLVM)
include(AddMLIR)

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${MLIR_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/lib)

add_subdirectory(include)

function(add_toy_object name)
  cmake_parse_arguments(ARG "" "" "DEPENDS;LINK_LIBS" ${ARGN})
  add_library(${name} OBJECT)
  target_sources(${name}
    PRIVATE ${ARG_UNPARSED_ARGUMENTS}
    INTERFACE $<TARGET_OBJECTS:${name}>
  )

  if(ARG_DEPENDS)
    add_dependencies(${name} ${ARG_DEPENDS})
  endif()

  if(ARG_LINK_LIBS)
    target_link_libraries(${name} PUBLIC ${ARG_LINK_LIBS})
  endif()
endfunction(add_toy_object)

set_property(GLOBAL PROPERTY TOY_LIBS "")
function(add_toy_library name)
  set_property(GLOBAL APPEND PROPERTY TOY_LIBS ${name})
  add_toy_object(${name} ${ARGN})
  llvm_update_compile_flags(${name})
endfunction(add_toy_library)

add_subdirectory(lib)
add_subdirectory(bin)
