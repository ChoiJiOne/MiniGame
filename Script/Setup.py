import sys
import os

def read_text_file(path)->str:
    with open(path, 'r') as file:
        content = file.read()
        return content

def write_text_file(path, text):
    with open(path, 'w') as file:
        file.write(text)

def generate_script_files(project_name):
    root_path = os.getcwd()
    path = root_path + "\\GameMaker\\"
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

if __name__ == "__main__":
    project_name = "Tetris3D"

    generate_script_files(project_name)

    print(f"\nSuccessed setup {project_name}!")
