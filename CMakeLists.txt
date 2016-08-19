cmake_minimum_required(VERSION 3.0)

project(libblockdsp)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-std=c++11")

include_directories(/usr/local/include "Audio Manager/" "BlockDSP/" "Code Generation/" "Logger/")
file(GLOB SOURCES "Audio\ Manager/*.cpp" "Audio\ Manager/*.hpp" "BlockDSP/*.cpp" "BlockDSP/*.hpp" "Code\ Generation/*.cpp" "Code\ Generation.hpp" "Logger/*.cpp" "Logger/*.hpp")

add_library(blockdsp STATIC ${SOURCES})