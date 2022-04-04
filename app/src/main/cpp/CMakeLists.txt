# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.18.1)
set(CMAKE_CXX_STANDARD 17)

project("noisegen")

add_library(
        noisegen

        SHARED

        PerlinNoise.h
        BMPImage.h
        native-lib.cpp)

find_library(
        log-lib

        log)


target_link_libraries(
        noisegen

        ${log-lib})