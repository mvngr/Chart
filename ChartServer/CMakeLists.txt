cmake_minimum_required(VERSION 3.5)

project(ChartServer LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON) # Разрешаем работу с собственной папкой

set(FILE_STRUCTURE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/FileStructure)

include_directories(
    ${FILE_STRUCTURE_PATH}
    )

file(GLOB_RECURSE SRC_H
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${FILE_STRUCTURE_PATH}/*.h
    )

file(GLOB_RECURSE SRC_CPP
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${FILE_STRUCTURE_PATH}/*.cpp
    )

add_executable(${PROJECT_NAME} WIN32 ${SRC_H} ${SRC_CPP})
target_link_libraries(ChartServer Qt5::Core Qt5::Network)
