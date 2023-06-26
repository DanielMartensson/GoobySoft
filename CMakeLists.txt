cmake_minimum_required(VERSION 3.18)
project(GoobySoft)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

file(GLOB_RECURSE SOURCES "GoobySoft/*.cpp")

file(GLOB_RECURSE INCLUDES "GoobySoft/*.h")

include_directories(${INCLUDES})

set(LIB_PATH "/usr/lib/x86_64-linux-gnu")

find_library(LIBMODBUS_LIBRARY NAMES modbus PATHS /usr/lib/x86_64-linux-gnu)

if (LIBMODBUS_LIBRARY)
    target_link_libraries(din_mål ${LIBMODBUS_LIBRARY})
else()
    message(FATAL_ERROR "Libmodbus library not found")
endif()

find_library(LIBMYSQLCPPCONN_LIBRARY NAMES mysqlcppconn PATHS ${LIB_PATH})
target_link_libraries(GoobySoft LIBMYSQLCPPCONN_LIBRARY)

find_library(LIBOPENGL_LIBRARY NAMES OpenGl PATHS ${LIB_PATH})
target_link_libraries(GoobySoft LIBOPENGL_LIBRARY)

find_library(LIBGLFW3_LIBRARY NAMES glfw3 PATHS ${LIB_PATH})
target_link_libraries(GoobySoft LIBGLFW3_LIBRARY)

add_executable(GoobySoft ${SOURCES})