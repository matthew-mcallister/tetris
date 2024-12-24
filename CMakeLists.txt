cmake_minimum_required(VERSION 3.14)

project(
    tetris
    VERSION 0.1.0
    DESCRIPTION "Tetris LED sign"
    LANGUAGES CXX
)

add_library(RpiRgbLedMatrix STATIC IMPORTED)
set_target_properties(RpiRgbLedMatrix PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/rpi-rgb-led-matrix/lib/librgbmatrix.a")


add_executable(tetris
    src/main.cpp
)
target_compile_features(tetris PRIVATE cxx_std_20)
target_link_libraries(tetris PRIVATE RpiRgbLedMatrix)
target_include_directories(tetris PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
