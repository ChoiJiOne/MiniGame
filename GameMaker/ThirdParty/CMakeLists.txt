# CMake의 최소 버전을 명시합니다.
cmake_minimum_required(VERSION 3.27)

add_subdirectory(glad)
add_subdirectory(miniaudio)
add_subdirectory(SDL)
add_subdirectory(stb)

set_target_properties(SDL2 SDL2main sdl_headers_copy glad stb miniaudio PROPERTIES FOLDER "ThirdParty")