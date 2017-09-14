cmake_minimum_required(VERSION 3.8)
project(DecoCLI)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp Utilities.cpp Utilities.h GFDeco.cpp GFDeco.h Deco.cpp Deco.h BMDeco.cpp BMDeco.h)
add_executable(DecoCLI ${SOURCE_FILES})