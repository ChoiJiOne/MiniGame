import sys
import os

def read_text_file(path)->str:
    with open(path, 'r') as file:
        content = file.read()
        return content

def write_text_file(path, text):
    with open(path, 'w') as file:
        file.write(text)

def is_already_setup(root_path, project_name):
    project_path = root_path + "\\" + project_name
    check_files = [
        root_path + "\\.gitignore",
        root_path + "\\Build_Debug.bat",
        root_path + "\\Build_MinSizeRel.bat",
        root_path + "\\Build_RelWithDebInfo.bat",
        root_path + "\\Build_Release.bat",
        root_path + "\\CMakeLists.txt",
        root_path + "\\GenerateProjectFiles.bat",
        root_path + "\\HotReload.bat",
        root_path + "\\LICENSE.txt",
        root_path + "\\Package_Debug.bat",
        root_path + "\\Package_MinSizeRel.bat",
        root_path + "\\Package_RelWithDebInfo.bat",
        root_path + "\\Package_Release.bat",

        project_path + "\\CMakeLists.txt",
        project_path + "\\Src\\Main.cpp",
    ]

    for check_file in check_files:
        is_already_exist = os.path.exists(check_file)
        output_message = f"[CHECK] {check_file} => "

        if is_already_exist:
            output_message += "Failed."
        else:
            output_message += "Ok."
        print(output_message)
            
        if is_already_exist:
            return True
        
    return False

def generate_files(root_path, project_name):
    path = root_path + "\\MiniGame\\"
    project_path = root_path + "\\" + project_name

    modes = [ "Debug", "MinSizeRel", "Release", "RelWithDebInfo", ]

    build_scripts = {}
    build_script_template = read_text_file(path + "Script\\BuildSolution_Template.bat")
    for mode in modes:
        script = build_script_template.format(MODE=mode)
        build_scripts[mode] = script

    package_scripts = {}
    package_script_template = read_text_file(path + "Script\\Package_Template.bat")
    for mode in modes:
        script = package_script_template.format(MODE=mode)
        package_scripts[mode] = script

    project_setting_scripts = {}
    project_setting_script_template = read_text_file(path + "Script\\GenerateProjectFiles_Template.bat")
    project_setting_scripts["HotReload"] = project_setting_script_template.format(NAME=project_name, OPTION="Off")
    project_setting_scripts["GenerateProjectFiles"] = project_setting_script_template.format(NAME=project_name, OPTION="On")

    solution_script_template = read_text_file(path + "CMake\\Solution_CMakeLists_Template.txt")
    solution_script = solution_script_template.replace("${SOLUTION_NAME}", project_name, 2)

    project_script_template = read_text_file(path + "CMake\\Project_CMakeLists_Template.txt")
    project_script = project_script_template.replace("{NAME}", project_name, 1)

    gitignore_file = read_text_file(path + "Misc\\.gitignore")
    license_file = read_text_file(path + "Misc\\LICENSE.txt")

    generate_target_files = {}

    for mode in modes:
        generate_target_files[root_path + "\\Build_" + mode + ".bat"] = build_scripts[mode]
        generate_target_files[root_path + "\\Package_" + mode + ".bat"] = package_scripts[mode]

    generate_target_files[root_path + "\\.gitignore"] = gitignore_file
    generate_target_files[root_path + "\\CMakeLists.txt"] = solution_script
    generate_target_files[root_path + "\\GenerateProjectFiles.bat"] = project_setting_scripts["GenerateProjectFiles"]
    generate_target_files[root_path + "\\HotReload.bat"] = project_setting_scripts["HotReload"]
    generate_target_files[root_path + "\\LICENSE.txt"] = license_file
    generate_target_files[project_path + "\\CMakeLists.txt"] = project_script
    generate_target_files[project_path + "\\Src\\Main.cpp"] = f"""#include <cstdint>
#include <Windows.h>

#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
#include <crtdbg.h>
#endif

int32_t WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int32_t nCmdShow)
{{
#if defined(DEBUG_MODE) || defined(RELEASE_MODE) || defined(DEVELOPMENT_MODE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	return 0;
}}
"""

    for path, text in generate_target_files.items():
        write_text_file(path, text)

if __name__ == "__main__":
    root_path = os.getcwd()
    project_name = sys.argv[1]

    if not is_already_setup(root_path, project_name):
        generate_files(root_path, project_name)
        print(f"\nSuccessed Setup '{project_name}'!")