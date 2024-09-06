# CMake의 최소 버전을 명시합니다.
cmake_minimum_required(VERSION 3.27)

add_subdirectory(AtlasViewer)

set_target_properties(
    AtlasViewer
    PROPERTIES FOLDER "Tool"
)