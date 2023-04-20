cmake_minimum_required(VERSION 3.18)
project(ChessMoveInterface)

# Includes
set(CMI_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/includes) # For conveniance
set(CMI_INCLUDE_DIR ${CMI_INCLUDE_DIR} PARENT_SCOPE) # To be used by other projects with add_subdirectory()
file(MAKE_DIRECTORY ${CMI_INCLUDE_DIR})

# Copy all includes
file(GLOB_RECURSE HEADER_FILES src/*.hpp)
foreach(FILE ${HEADER_FILES})
file(RELATIVE_PATH FILE_SUBDIR ${CMAKE_CURRENT_SOURCE_DIR}/src ${FILE})
configure_file(${FILE} ${CMI_INCLUDE_DIR}/${FILE_SUBDIR} COPYONLY)
endforeach(FILE ${HEADER_FILES})

# Add target if not exists
# This way ChessMoveInterface can be used in multiple submodules
include_directories(src)
file(GLOB_RECURSE SRC_CPP_FILES src/*.cpp)
if(NOT TARGET ChessMoveInterface)
    add_library(ChessMoveInterface SHARED ${SRC_CPP_FILES})
endif()

# Unit tests
set(COMPILE_TESTS OFF CACHE BOOL "Should unit tests be compiled")
if(COMPILE_TESTS)
    enable_testing()
    add_subdirectory(./tests)
endif()