# CMake의 최소 버전을 명시합니다.
cmake_minimum_required(VERSION 3.27)

add_subdirectory(glad)
add_subdirectory(imgui)
add_subdirectory(jsoncpp)
add_subdirectory(ktx)
add_subdirectory(miniaudio)
add_subdirectory(SDL)
add_subdirectory(stb)

set_target_properties(
    glad 
    imgui 
    jsoncpp 
    miniaudio 
    SDL2 
    SDL2main 
    sdl_headers_copy 
    stb 
    astcenc-avx2-static
    ktx
    ktx_read
    ktx_version
    obj_basisu_cbind
    objUtil
    PROPERTIES FOLDER "ThirdParty"
)