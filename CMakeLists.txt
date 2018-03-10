cmake_minimum_required(VERSION 2.8.3)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

project(TD)

add_subdirectory(game)