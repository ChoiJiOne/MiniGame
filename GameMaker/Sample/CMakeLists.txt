cmake_minimum_required(VERSION 3.27)

add_subdirectory(01.Initialize)
add_subdirectory(02.Triangle)
add_subdirectory(03.Texture)
add_subdirectory(04.MultiTexture)
add_subdirectory(05.KTX)
add_subdirectory(06.Grid2D)
add_subdirectory(07.Rendering2D)

set_target_properties(
    01.Initialize
    02.Triangle
    03.Texture
    04.MultiTexture
    05.KTX
    06.Grid2D
    07.Rendering2D
    PROPERTIES FOLDER "Sample"
)