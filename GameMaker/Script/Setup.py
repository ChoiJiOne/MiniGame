import sys
import os

def is_already_setup()->bool:
    for setup_target_file in setup_target_files.keys():
        is_already_exist = os.path.exists(setup_target_file)
        output_message = f"[CHECK] {setup_target_file} => "

        if is_already_exist:
            output_message += "Failed."
            print(output_message)
            return True
        
        output_message += "Ok."
        print(output_message)

    return False

def write_text_file(path, text):
    with open(path, 'w') as file:
        file.write(text)
    
if __name__ == "__main__":
    root_path = os.getcwd()
    project_name = sys.argv[1]
    option = sys.argv[2]
    project_path = root_path + "\\" + project_name
    setup_target_files = {
        f"{project_path}\\CMakeLists.txt" : f"""cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_NAME \"{project_name}\")

set(PROJECT_PATH ${{CMAKE_CURRENT_SOURCE_DIR}})
set(PROJECT_INCLUDE_PATH \"${{PROJECT_PATH}}/Inc\")
set(PROJECT_SOURCE_PATH \"${{PROJECT_PATH}}/Src\")

file(GLOB_RECURSE PROJECT_INCLUDE_FILE \"${{PROJECT_INCLUDE_PATH}}/**\")
file(GLOB_RECURSE PROJECT_SOURCE_FILE \"${{PROJECT_SOURCE_PATH}}/**\")

add_executable(${{PROJECT_NAME}} WIN32 ${{PROJECT_INCLUDE_FILE}} ${{PROJECT_SOURCE_FILE}})

target_include_directories(${{PROJECT_NAME}} PUBLIC ${{PROJECT_INCLUDE_PATH}})
target_link_libraries(${{PROJECT_NAME}} PUBLIC GameMaker)

target_compile_definitions(
    ${{PROJECT_NAME}}
    PUBLIC
    $<$<CONFIG:Debug>:DEBUG_MODE>
    $<$<CONFIG:Release>:RELEASE_MODE>
    $<$<CONFIG:RelWithDebInfo>:RELWITHDEBINFO_MODE>
    $<$<CONFIG:MinSizeRel>:MINSIZEREL_MODE>
)

set_property(TARGET ${{PROJECT_NAME}} PROPERTY MSVC_RUNTIME_LIBRARY \"MultiThreaded$<$<CONFIG:Debug>:Debug>\")
set_property(TARGET ${{PROJECT_NAME}} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY \"${{CMAKE_CURRENT_SOURCE_DIR}}/../\")

source_group(TREE \"${{PROJECT_INCLUDE_PATH}}\" PREFIX \"${{PROJECT_NAME}}/Inc\" FILES ${{PROJECT_INCLUDE_FILE}})
source_group(TREE \"${{PROJECT_SOURCE_PATH}}\" PREFIX \"${{PROJECT_NAME}}/Src\" FILES ${{PROJECT_SOURCE_FILE}})""",
        f"{root_path}\\.gitignore" : """.vs
Win64
Solution
Cache

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
*.rc""",
        f"{root_path}\\Build_Debug.bat" : """@ECHO OFF
SET MODE=Debug
SET SCRIPT_PATH=GameMaker\\Script
SET SCRIPT=%SCRIPT_PATH%\\BuildSolution.bat

%SCRIPT% %MODE%
""",
        f"{root_path}\\Build_MinSizeRel.bat" : """@ECHO OFF

SET MODE=MinSizeRel
SET SCRIPT_PATH=GameMaker\\Script
SET SCRIPT=%SCRIPT_PATH%\\BuildSolution.bat

%SCRIPT% %MODE%
""",
        f"{root_path}\\Build_RelWithDebInfo.bat" : """@ECHO OFF

SET MODE=RelWithDebInfo
SET SCRIPT_PATH=GameMaker\\Script
SET SCRIPT=%SCRIPT_PATH%\\BuildSolution.bat

%SCRIPT% %MODE%
""",
        f"{root_path}\\Build_Release.bat" : """@ECHO OFF

SET MODE=Release
SET SCRIPT_PATH=GameMaker\\Script
SET SCRIPT=%SCRIPT_PATH%\\BuildSolution.bat

%SCRIPT% %MODE%
""",
        f"{root_path}\\CMakeLists.txt" : f"""cmake_minimum_required(VERSION 3.27)

project({project_name})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${{CMAKE_BINARY_DIR}})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${{CMAKE_BINARY_DIR}})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${{CMAKE_BINARY_DIR}})

set(PROJECT_NAME \"{project_name}\")

add_subdirectory(GameMaker)
add_subdirectory(${{PROJECT_NAME}})

set_property(DIRECTORY ${{CMAKE_CURRENT_SOURCE_DIR}} PROPERTY VS_STARTUP_PROJECT ${{PROJECT_NAME}})
""",
        f"{root_path}\\GenerateProjectFiles.bat" : f"""@ECHO OFF

SET PROJECT_NAME={project_name}
SET SCRIPT_PATH=GameMaker\\Script
SET SCRIPT=%SCRIPT_PATH%\\GenerateProjectFiles.bat

%SCRIPT% %PROJECT_NAME% On
""",
        f"{root_path}\\HotReload.bat" : f"""@ECHO OFF

SET PROJECT_NAME={project_name}
SET SCRIPT_PATH=GameMaker\\Script
SET SCRIPT=%SCRIPT_PATH%\\GenerateProjectFiles.bat

%SCRIPT% %PROJECT_NAME% Off
""",
        f"{root_path}\\Package_Debug.bat" : f"""@ECHO OFF

SET MODE=Debug
SET SCRIPT_PATH=GameMaker\\Script
SET SCRIPT=%SCRIPT_PATH%\\Package.bat

%SCRIPT% %MODE%
""",
        f"{root_path}\\Package_MinSizeRel.bat" : f"""@ECHO OFF

SET MODE=MinSizeRel
SET SCRIPT_PATH=GameMaker\\Script
SET SCRIPT=%SCRIPT_PATH%\\Package.bat

%SCRIPT% %MODE%
""",
        f"{root_path}\\Package_RelWithDebInfo.bat" : f"""@ECHO OFF

SET MODE=RelWithDebInfo
SET SCRIPT_PATH=GameMaker\\Script
SET SCRIPT=%SCRIPT_PATH%\\Package.bat

%SCRIPT% %MODE%
""",
        f"{root_path}\\Package_Release.bat" : f"""@ECHO OFF

SET MODE=Release
SET SCRIPT_PATH=GameMaker\\Script
SET SCRIPT=%SCRIPT_PATH%\\Package.bat

%SCRIPT% %MODE%
""",
        f"{project_path}\\Src\\Main.cpp": f"""#include <cstdint>
#include <Windows.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

#include "GameEngine.h"
#include "RenderManager.h"

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	GameMaker::WindowParam param {{ \"{project_name}\", 100, 100, 800, 600, false, false }};
	GameMaker::GameEngine::Init(param);

	GameMaker::GameEngine::RunLoop(
		[&](float deltaSeconds) 
		{{
			GameMaker::RenderManager::Get().BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);
			GameMaker::RenderManager::Get().EndFrame();
		}}
	);
	GameMaker::GameEngine::Shutdown();

	return 0;
}}
""",
        f"{root_path}\\LICENSE.txt" : """The MIT License (MIT)

Copyright GameMaker

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
""",
    }

    if is_already_setup():
        print(f"Failed setup {project_name}!")
        sys.exit()

    gitignore_file = setup_target_files[f"{root_path}\\.gitignore"]
    if option == "--ignore":
        gitignore_file += f"\n\n{project_name}\n"
        gitignore_file += "Build_Debug.bat\n"
        gitignore_file += "Build_MinSizeRel.bat\n"
        gitignore_file += "Build_RelWithDebInfo.bat\n"
        gitignore_file += "Build_Release.bat\n"
        gitignore_file += "CMakeLists.txt\n"
        gitignore_file += "GenerateProjectFiles.bat\n"
        gitignore_file += "HotReload.bat\n"
        gitignore_file += "LICENSE.txt\n"
        gitignore_file += "Package_Debug.bat\n"
        gitignore_file += "Package_MinSizeRel.bat\n"
        gitignore_file += "Package_RelWithDebInfo.bat\n"
        gitignore_file += "Package_Release.bat"
        
        setup_target_files[f"{root_path}\\.gitignore"] = gitignore_file

    for path, text in setup_target_files.items():
        write_text_file(path, text)

    print(f"Successed setup {project_name}!")