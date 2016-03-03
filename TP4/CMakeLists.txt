cmake_minimum_required(VERSION 2.6)
project(geonum_TP4)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -D_REENTRANT -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
set(CMAKE_CXX_COMPILER "g++")

include_directories(../eigen-3.2.7/)
set(SOURCES main.cpp TP4.h)

file(
    GLOB
    source_files
    *.cpp
)

add_executable( geonum_TP4 ${source_files} )
