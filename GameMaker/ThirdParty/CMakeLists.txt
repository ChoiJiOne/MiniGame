# CMake의 최소 버전을 명시합니다.
cmake_minimum_required(VERSION 3.27)

add_subdirectory(SDL)

set_target_properties(SDL2 SDL2main sdl_headers_copy PROPERTIES FOLDER "ThirdParty")