cmake_minimum_required(VERSION 3.18)
project(GoobySoft)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "GoobySoft/*.cpp")

file(GLOB_RECURSE INCLUDES "GoobySoft/*.h")

include_directories(${INCLUDES})

add_executable(GoobySoft ${SOURCES})