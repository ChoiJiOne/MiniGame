# CMake의 최소 버전을 명시합니다.
cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LIB_NAME "imgui")

set(LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(LIB_INCLUDE "${LIB_PATH}/include")
set(LIB_SOURCE "${LIB_PATH}/src")

file(GLOB_RECURSE LIB_INCLUDE_FILE "${LIB_INCLUDE}/*")
file(GLOB_RECURSE LIB_SOURCE_FILE "${LIB_SOURCE}/*")

add_library(${LIB_NAME} STATIC ${LIB_INCLUDE_FILE} ${LIB_SOURCE_FILE})

target_include_directories(${LIB_NAME} PUBLIC ${LIB_INCLUDE})

set_property(TARGET ${LIB_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

source_group(include FILES ${LIB_INCLUDE_FILE})
source_group(src FILES ${LIB_SOURCE_FILE})