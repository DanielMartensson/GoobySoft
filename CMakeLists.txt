cmake_minimum_required(VERSION 3.18)
project(GoobySoft)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

file(GLOB_RECURSE SOURCES "GoobySoft/*.cpp")

file(GLOB_RECURSE INCLUDES "GoobySoft/*.h")

include_directories(${INCLUDES})

add_executable(GoobySoft ${SOURCES})