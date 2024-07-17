cmake_minimum_required(VERSION 3.27)

add_subdirectory(01.Initialize)
add_subdirectory(02.Triangle)
add_subdirectory(03.Texture)

set_target_properties(
    01.Initialize
    02.Triangle
    03.Texture
    PROPERTIES FOLDER "Sample"
)