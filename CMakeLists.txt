cmake_minimum_required(VERSION 3.28.3)
project(GoobySoft)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# optimeringsflaggor

file(GLOB_RECURSE SOURCES "GoobySoft/*.cpp" "GoobySoft/*.c")

file(GLOB_RECURSE INCLUDES "GoobySoft/*.h")

add_executable(GoobySoft ${SOURCES})

include_directories(${INCLUDES})

# Hitta och länka mot MySQL C++ Connector
find_package(unofficial-mysql-connector-cpp CONFIG REQUIRED)
if(unofficial-mysql-connector-cpp_FOUND)
    message("MySQL C++ Connector library found")
    target_link_libraries(GoobySoft PRIVATE unofficial::mysql-connector-cpp::connector)
else()
    message(FATAL_ERROR "MySQL C++ Connector library not found")
endif()

# Hitta och länka mot libdl
if(UNIX)
    find_library(LIBDL_LIBRARY NAMES dl)
    if (LIBDL_LIBRARY)
        message("libdl library found")
        target_link_libraries(GoobySoft PRIVATE ${LIBDL_LIBRARY})
    else()
        message(FATAL_ERROR "libdl library not found")
    endif()
endif()

# Hitta och länka mot SDL3
find_library(LIBSDL3_LIBRARY NAMES SDL3)
if (LIBSDL3_LIBRARY)
    message("SDL3 library found")
    target_link_libraries(GoobySoft PRIVATE ${LIBSDL3_LIBRARY})
else()
    message(FATAL_ERROR "SDL3 library not found")
endif()

# Hitta och länka mot Pthreads
#find_package(Threads REQUIRED)
find_package(PThreads4W REQUIRED)
if(Threads_FOUND)
    message("PThread library found")
    #target_link_libraries(GoobySoft PRIVATE Threads::Threads)
    target_link_libraries(GoobySoft PRIVATE PThreads4W::PThreads4W)
else()
    message(FATAL_ERROR "PThread library not found")
endif()

# Hitta och länka mot boost-asio
find_package(Boost CONFIG REQUIRED COMPONENTS system)
if (Boost_FOUND)
    message("Boost library found")
    target_link_libraries(GoobySoft PRIVATE Boost::system)
else()
    message(FATAL_ERROR "Boost library not found")
endif()

# Hitta och länka mot boost-filesystem
find_package(Boost CONFIG REQUIRED COMPONENTS filesystem)
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
    target_include_directories(GoobySoft PRIVATE ${OPENGL_INCLUDE_DIR})
else()
    message(FATAL_ERROR "OpenGL library not found")
endif()

# Hitta och länka mot OpenCV
find_package(OpenCV REQUIRED)
if (OpenCV_FOUND)
    message("OpenCV library found")
    target_link_libraries(GoobySoft PRIVATE ${OpenCV_LIBS})
else()
    message(FATAL_ERROR "OpenCV library not found")
endif()
