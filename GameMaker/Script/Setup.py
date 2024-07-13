import sys
import os

def is_already_setup()->bool:
    for setup_target_file in setup_target_files:
        is_already_exist = os.path.exists(setup_target_file)

        output_message = f"[CHECK] {setup_target_file} => "

        if is_already_exist:
            output_message += "Failed."
            print(output_message)
            return True
        
        output_message += "Ok."
        print(output_message)
        
    return False
    
if __name__ == "__main__":
    root_path = os.getcwd()
    project_name = sys.argv[1]
    option = sys.argv[2]
    project_path = root_path + "\\" + project_name

    setup_target_files = [
        f"{project_path}\\CMakeLists.txt",
        f"{root_path}\\.gitignore",
        f"{root_path}\\Build_Debug.bat",
        f"{root_path}\\Build_MinSizeRel.bat",
        f"{root_path}\\Build_RelWithDebInfo.bat",
        f"{root_path}\\Build_Release.bat",
        f"{root_path}\\CMakeLists.txt",
        f"{root_path}\\GenerateProjectFiles.bat",
        f"{root_path}\\HotReload.bat",
        f"{root_path}\\Package_Debug.bat",
        f"{root_path}\\Package_MinSizeRel.bat",
        f"{root_path}\\Package_RelWithDebInfo.bat",
        f"{root_path}\\Package_Release.bat",
        f"{root_path}\\LICENSE.txt"
    ]

    if is_already_setup():
        print(f"Failed setup {project_name}!")
    
    print(f"Successed setup {project_name}!")