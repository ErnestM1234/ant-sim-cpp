cmake_minimum_required(VERSION 3.16)
project(AntSim LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(antsim ${SOURCES})

target_include_directories(antsim PRIVATE src)
target_link_libraries(antsim PRIVATE SFML::Graphics SFML::Window SFML::System)
