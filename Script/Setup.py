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
    project_name = sys.argv[1]
    option = sys.argv[2]
    
    root_path = os.getcwd()
    template_path = root_path + "\\GameMaker\\Template\\"
    project_path = root_path + "\\" + project_name

    if is_already_setup():
        print(f"Failed setup {project_name}!")
        sys.exit()

    template_file_path = f"{root_path}"
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