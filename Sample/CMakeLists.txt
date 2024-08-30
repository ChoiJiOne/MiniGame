cmake_minimum_required(VERSION 3.27)

add_subdirectory(01.Initialize)
add_subdirectory(02.Crash)
add_subdirectory(03.Triangle)

set_target_properties(
    01.Initialize
    02.Crash
    03.Triangle
    PROPERTIES FOLDER "Sample"
)