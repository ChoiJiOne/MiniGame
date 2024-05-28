# CMake의 최소 버전을 명시합니다.
cmake_minimum_required(VERSION 3.27)

add_subdirectory(cgltf)
add_subdirectory(glad)
add_subdirectory(imgui)
add_subdirectory(jsoncpp)
add_subdirectory(lua)
add_subdirectory(miniaudio)
add_subdirectory(Remotery)
add_subdirectory(stb)

set_target_properties(cgltf glad imgui jsoncpp lua miniaudio Remotery stb PROPERTIES FOLDER "ThirdParty")