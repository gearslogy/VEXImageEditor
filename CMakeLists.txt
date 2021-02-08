cmake_minimum_required(VERSION 3.5)
project(VEXImageEditor)


# Houdini
list( APPEND CMAKE_PREFIX_PATH "C:/Program Files/Side Effects Software/Houdini 18.5.408/toolkit/cmake" )
find_package( Houdini REQUIRED )




FILE(GLOB
        SRCFILES
        src/*.h src/*.cpp)

add_executable(VexImage ${SRCFILES} src/ImageItem.cpp src/ImageItem.h)
target_link_libraries(VexImage Houdini)