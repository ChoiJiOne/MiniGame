cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_NAME "RenderModule")

set(MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(MODULE_INCLUDE "${MODULE_PATH}/Inc")
set(MODULE_SOURCE "${MODULE_PATH}/Src")
set(SHADER_SOURCE "${MODULE_PATH}/../../Resource/Shader")

file(GLOB_RECURSE MODULE_INCLUDE_FILE "${MODULE_INCLUDE}/*")
file(GLOB_RECURSE MODULE_SOURCE_FILE "${MODULE_SOURCE}/*")
file(GLOB_RECURSE SHADER_SOURCE_FILE "${SHADER_SOURCE}/*")

add_library(${PROJECT_NAME} STATIC ${MODULE_INCLUDE_FILE} ${MODULE_SOURCE_FILE} ${SHADER_SOURCE_FILE})

target_include_directories(${PROJECT_NAME} PUBLIC ${MODULE_INCLUDE})
target_link_libraries(
    ${PROJECT_NAME}
    opengl32.lib
    glad
    imgui
    stb
    AnimationModule
    CrashModule
    FileModule
    MathModule
)

target_compile_definitions(
    ${PROJECT_NAME} 
    PUBLIC
    $<$<CONFIG:Debug>:DEBUG_MODE>
    $<$<CONFIG:Release>:RELEASE_MODE>
    $<$<CONFIG:RelWithDebInfo>:DEVELOPMENT_MODE>
    $<$<CONFIG:MinSizeRel>:SHIPPING_MODE>
)

set(VALIDATOR_PATH "${MODULE_PATH}/../../Script/ShaderValidator.bat")
foreach (SHADER_SOURCE_FILE IN LISTS SHADER_SOURCE_FILE)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${VALIDATOR_PATH} ${SHADER_SOURCE_FILE}
    )
endforeach()

set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

source_group(Inc FILES ${MODULE_INCLUDE_FILE})
source_group(Src FILES ${MODULE_SOURCE_FILE})
source_group(Shader FILES ${SHADER_SOURCE_FILE})