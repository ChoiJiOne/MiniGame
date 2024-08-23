cmake_minimum_required(VERSION 3.27)

add_subdirectory(01.Initialize)
add_subdirectory(02.Triangle)
add_subdirectory(03.Texture)
add_subdirectory(04.MultiTexture)
add_subdirectory(05.Grid2D)
add_subdirectory(06.Grid3D)
add_subdirectory(07.Cube)
add_subdirectory(08.Sprite2D)
add_subdirectory(09.String2D)
add_subdirectory(10.GLTF)
add_subdirectory(11.File)

set_target_properties(
    01.Initialize
    02.Triangle
    03.Texture
    04.MultiTexture
    05.Grid2D
    06.Grid3D
    07.Cube
    08.Sprite2D
    09.String2D
    10.GLTF
    11.File
    PROPERTIES FOLDER "Sample"
)