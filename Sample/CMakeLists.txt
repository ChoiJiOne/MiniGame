cmake_minimum_required(VERSION 3.27)

add_subdirectory(01.Initialize)
add_subdirectory(02.Crash)
add_subdirectory(03.Triangle)
add_subdirectory(04.Texture)
add_subdirectory(05.DDS)
add_subdirectory(06.Geometry2D)
add_subdirectory(10.Sound)

set_target_properties(
    01.Initialize
    02.Crash
    03.Triangle
    04.Texture
    05.DDS
    06.Geometry2D
    10.Sound
    PROPERTIES FOLDER "Sample"
)