cmake_minimum_required(VERSION 3.18)
project(GoobySoft)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

file(GLOB_RECURSE SOURCES "GoobySoft/*.cpp")

file(GLOB_RECURSE INCLUDES "GoobySoft/*.h")

add_executable(GoobySoft ${SOURCES})

include_directories(${INCLUDES})

# Hitta och länka mot libmodbus
find_library(LIBMODBUS_LIBRARY NAMES modbus PATHS /usr/lib/x86_64-linux-gnu)
if (LIBMODBUS_LIBRARY)
    target_link_libraries(GoobySoft PRIVATE ${LIBMODBUS_LIBRARY})
else()
    message(FATAL_ERROR "Libmodbus library not found")
endif()

# Hitta och länka mot MySQL C++ Connector
find_library(LIBMYSQLCPPCONN_LIBRARY NAMES mysqlcppconn8 PATHS /usr/lib/x86_64-linux-gnu)
if (LIBMYSQLCPPCONN_LIBRARY)
    target_link_libraries(GoobySoft PRIVATE ${LIBMYSQLCPPCONN_LIBRARY})
else()
    message(FATAL_ERROR "MySQL C++ Connector library not found")
endif()

# Hitta och länka mot boost-asio
find_package(Boost REQUIRED COMPONENTS system)
if (Boost_FOUND)
    target_link_libraries(GoobySoft PRIVATE Boost::system)
else()
    message(FATAL_ERROR "Boost library not found")
endif()

# Hitta och länka mot boost-filesystem
find_package(Boost REQUIRED COMPONENTS filesystem)
if (Boost_FOUND)
    target_link_libraries(GoobySoft PRIVATE Boost::filesystem)
else()
    message(FATAL_ERROR "Boost Filesystem library not found")
endif()

# Hitta och länka mot OpenGL
find_package(OpenGL REQUIRED)
if (OpenGL_FOUND)
    target_link_libraries(GoobySoft PRIVATE OpenGL::GL)
else()
    message(FATAL_ERROR "OpenGL library not found")
endif()

# Hitta och länka mot GLFW3
find_package(glfw3 REQUIRED)
if (glfw3_FOUND)
    target_link_libraries(GoobySoft PRIVATE glfw)
else()
    message(FATAL_ERROR "GLFW3 library not found")
endif()