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
find_library(LIBMODBUS_LIBRARY NAMES modbus)
if (LIBMODBUS_LIBRARY)
    message("Libmodbus library found")
    target_link_libraries(GoobySoft PRIVATE ${LIBMODBUS_LIBRARY})
else()
    message(FATAL_ERROR "Libmodbus library not found")
endif()

# Hitta och länka mot MySQL C++ Connector
find_library(LIBMYSQLCPPCONN_LIBRARY NAMES mysqlcppconn8)
if (LIBMYSQLCPPCONN_LIBRARY)
    message("MySQL C++ Connector 8 library found")
    target_link_libraries(GoobySoft PRIVATE ${LIBMYSQLCPPCONN_LIBRARY})
else()
    message(FATAL_ERROR "MySQL C++ Connector library not found")
endif()

# Hitta och länka mot libdl
find_library(LIBDL_LIBRARY NAMES dl)
if (LIBDL_LIBRARY)
    message("libdl library found")
    target_link_libraries(GoobySoft PRIVATE ${LIBDL_LIBRARY})
else()
    message(FATAL_ERROR "libdl library not found")
endif()

# Hitta och länka mot Pthreads
find_package(Threads REQUIRED)
if(Threads_FOUND)
    message("PThread library found")
    target_link_libraries(GoobySoft PRIVATE Threads::Threads)
else()
    message(FATAL_ERROR "PThread library not found")
endif()

# Hitta och länka mot boost-asio
find_package(Boost REQUIRED COMPONENTS system)
if (Boost_FOUND)
    message("Boost library found")
    target_link_libraries(GoobySoft PRIVATE Boost::system)
else()
    message(FATAL_ERROR "Boost library not found")
endif()

# Hitta och länka mot boost-filesystem
find_package(Boost REQUIRED COMPONENTS filesystem)
if (Boost_FOUND)
    message("Boost Filesystem library found")
    target_link_libraries(GoobySoft PRIVATE Boost::filesystem)
else()
    message(FATAL_ERROR "Boost Filesystem library not found")
endif()

# Hitta och länka mot OpenGL
find_package(OpenGL REQUIRED)
if (OpenGL_FOUND)
    message("OpenGL library found")
    target_link_libraries(GoobySoft PRIVATE OpenGL::GL)
else()
    message(FATAL_ERROR "OpenGL library not found")
endif()

# Hitta och länka mot GLFW3
find_package(glfw3 REQUIRED)
if (glfw3_FOUND)
    message("GLFW3 library found")
    target_link_libraries(GoobySoft PRIVATE glfw)
else()
    message(FATAL_ERROR "GLFW3 library not found")
endif()

