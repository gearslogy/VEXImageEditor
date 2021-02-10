cmake_minimum_required(VERSION 3.5)
project(VEXImageEditor)


# Houdini & Qt CMAKE_PREFIX_PATH
list( APPEND CMAKE_PREFIX_PATH "C:/Program Files/Side Effects Software/Houdini 18.5.462/toolkit/cmake" "D:/Qt/6.0.1/msvc2019_64")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Houdini package
find_package( Houdini REQUIRED )


# Qt package
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)



file(GLOB_RECURSE COMMON_SOURCE "*.cpp")
FILE(GLOB
        SRCFILES
        src/*.h src/*.cpp)
        
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/binary")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/binary")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/binary")

qt_add_executable(VexImage ${SRCFILES})
target_link_libraries(VexImage PRIVATE Houdini Qt${QT_VERSION_MAJOR}::Widgets)


