cmake_minimum_required(VERSION 3.18)
project(GoobySoft)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

file(GLOB_RECURSE SOURCES "GoobySoft/*.cpp")
file(GLOB_RECURSE INCLUDES "GoobySoft/*.h")

include_directories(${INCLUDES})

add_executable(GoobySoft ${SOURCES})

set(LIB_PATH "/usr/lib/x86_64-linux-gnu")

find_library(LIBMODBUS_LIBRARY modbus HINTS ${LIB_PATH})
if (LIBMODBUS_LIBRARY)
    message(STATUS "Found libmodbus: ${LIBMODBUS_LIBRARY}")
    target_link_libraries(GoobySoft PRIVATE ${LIBMODBUS_LIBRARY})
else()
    message(FATAL_ERROR "Libmodbus library not found")
endif()

find_library(LIBMYSQLCPPCONN_LIBRARY mysqlcppconn HINTS ${LIB_PATH})
if (LIBMYSQLCPPCONN_LIBRARY)
    message(STATUS "Found libmysqlcppconn: ${LIBMYSQLCPPCONN_LIBRARY}")
    target_link_libraries(GoobySoft PRIVATE ${LIBMYSQLCPPCONN_LIBRARY})
else()
    message(FATAL_ERROR "libmysqlcppconn library not found")
endif()

find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
    message(STATUS "Found OpenGL")
    target_link_libraries(GoobySoft PRIVATE ${OPENGL_LIBRARIES})
    include_directories(${OPENGL_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "OpenGL not found")
endif()

find_package(glfw3 REQUIRED)
if (glfw3_FOUND)
    message(STATUS "Found GLFW3")
    target_link_libraries(GoobySoft PRIVATE glfw)
    include_directories(${GLFW_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "GLFW3 not found")
endif()

