local name = arg[1]
local root = arg[2]
local project = arg[3]

function isValid(value)
    return value ~= nil
end

function writeFile(path, source)
    local file = io.open(path, "w")
    
    if file then
        file:write(source)
        file:close()
    end
end

if isValid(name) then
    local message = string.format("[NAME] %s", name)
    print(message)
else
    os.exit(1)
end

if isValid(root) then
    local message = string.format("[ROOT] %s", root)
    print(message)
else
    os.exit(1)
end

if isValid(project) then
    local message = string.format("[PROJECT] %s", project)
    print(message)
else
    os.exit(1)
end


local build_script = [[
@echo off

SET mode=%s
SET script=Engine\Script\BuildSolution.bat

%%script%% %%mode%%
]]

local project_solution_script = [[
@echo off

SET visualstudio=%s
SET project=%s
SET script=Engine\Script\GenerateProjectFiles.bat

%%script%% %%visualstudio%% %%project%% %s
]]

local package_script = [[
@echo off

SET mode=%s
SET script=Engine\Script\Package.bat

%%script%% %%mode%%
]]

local solution_script = [[
cmake_minimum_required(VERSION 3.27)

project(%s)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(PROJECT_NAME "%s")

add_subdirectory(Engine)
add_subdirectory(${PROJECT_NAME})
]]

local project_script = [[
cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_NAME "%s")

set(PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJECT_INCLUDE_PATH "${PROJECT_PATH}/Inc")
set(PROJECT_SOURCE_PATH "${PROJECT_PATH}/Src")

file(GLOB_RECURSE PROJECT_INCLUDE_FILE "${PROJECT_INCLUDE_PATH}/**")
file(GLOB_RECURSE PROJECT_SOURCE_FILE "${PROJECT_SOURCE_PATH}/**")

add_executable(${PROJECT_NAME} WIN32 ${PROJECT_INCLUDE_FILE} ${PROJECT_SOURCE_FILE})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_PATH})
target_link_libraries(
    ${PROJECT_NAME} 
    PUBLIC 
    AnimationModule
    AudioModule
    CrashModule
    FileModule
    GameModule
    MathModule
    PlatformModule
    RenderModule
)

target_compile_definitions(
    ${PROJECT_NAME} 
    PUBLIC
    $<$<CONFIG:Debug>:DEBUG_MODE>
    $<$<CONFIG:Release>:RELEASE_MODE>
    $<$<CONFIG:RelWithDebInfo>:DEVELOPMENT_MODE>
    $<$<CONFIG:MinSizeRel>:SHIPPING_MODE>
)

set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../")

source_group(TREE "${PROJECT_INCLUDE_PATH}" PREFIX "${PROJECT_NAME}/Inc" FILES ${PROJECT_INCLUDE_FILE})
source_group(TREE "${PROJECT_SOURCE_PATH}" PREFIX "${PROJECT_NAME}/Src" FILES ${PROJECT_SOURCE_FILE})
]]

local source_file = [[
#include <cstdint>
#include <Windows.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    return 0;
}
]]

local gitignore_file = [[
.vs
Win64
Solution

*.vcxproj.filters
*.vcxproj.user
*.tlog
*.pdb
*.recipe
*.ilk
*.log
*.idb
*.sln
*.vcxproj
*.aps
*.bin
*.rc
]]

writeFile("Build_Debug.bat", string.format(build_script, "Debug"))
writeFile("Build_MinSizeRel.bat", string.format(build_script, "MinSizeRel"))
writeFile("Build_Release.bat", string.format(build_script, "Release"))
writeFile("Build_RelWithDebInfo.bat", string.format(build_script, "RelWithDebInfo"))
writeFile("GenerateProjectFiles_vs2019.bat", string.format(project_solution_script, "vs2019", name, "On"))
writeFile("GenerateProjectFiles_vs2022.bat", string.format(project_solution_script, "vs2022", name, "On"))
writeFile("HotReload_vs2019.bat", string.format(project_solution_script, "vs2019", name, "Off"))
writeFile("HotReload_vs2022.bat", string.format(project_solution_script, "vs2022", name, "Off"))
writeFile("Package_Debug.bat", string.format(package_script, "Debug"))
writeFile("Package_MinSizeRel.bat", string.format(package_script, "MinSizeRel"))
writeFile("Package_Release.bat", string.format(package_script, "Release"))
writeFile("Package_RelWithDebInfo.bat", string.format(package_script, "RelWithDebInfo"))
writeFile("CMakeLists.txt", string.format(solution_script, name, name))
writeFile(string.format("%s/%s", name, "CMakeLists.txt"), string.format(project_script, name))
writeFile(string.format("%s/Src/Main.cpp", name), source_file)
writeFile(".gitignore", gitignore_file)