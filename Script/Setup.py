import sys
import os

def read_text_file(path)->str:
    with open(path, 'r') as file:
        content = file.read()
        return content

def write_text_file(path, text):
    with open(path, 'w') as file:
        file.write(text)
    
if __name__ == "__main__":
    project_name = "Tetris3D"

    root_path = os.getcwd()
    template_path = root_path + "\\GameMaker\\Template\\"
    project_path = root_path + "\\" + project_name

    modes = [ "Debug", "MinSizeRel", "Release", "RelWithDebInfo", ]

    build_scripts = {}
    build_script_template = read_text_file(template_path + "BuildScript.bat")
    for mode in modes:
        script = build_script_template.format(MODE=mode)
        build_scripts[mode] = script

    package_scripts = {}
    package_script_template = read_text_file(template_path + "PackageScript.bat")
    for mode in modes:
        script = package_script_template.format(MODE=mode)
        package_scripts[mode] = script

    project_scripts = {}
    project_script_template = read_text_file(template_path + "ProjectScript.bat")
    project_scripts["HotReload"] = project_script_template.format(NAME=project_name, OPTION="Off")
    project_scripts["GenerateProjectFiles"] = project_script_template.format(NAME=project_name, OPTION="On")

    solution_script_template = read_text_file(template_path + "SolutionScript.txt")
    solution_script = solution_script_template.replace("${SOLUTION_NAME}", project_name, 2)

    


    print(f"Successed setup {project_name}!")
