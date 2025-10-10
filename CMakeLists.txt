cmake_minimum_required(VERSION 3.18)
project(GoobySoft)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# Uncomment this if you want the debug mode activated
#add_compile_definitions(_GOOBYSOFT_DEBUG)

file(GLOB_RECURSE SOURCES "GoobySoft/*.cpp" "GoobySoft/*.c")

file(GLOB_RECURSE INCLUDES "GoobySoft/*.h")

add_executable(GoobySoft ${SOURCES})

include_directories(${INCLUDES})

# Hitta och länka mot MySQL C++ Connector
find_library(LIBMYSQLCPPCONN_LIBRARY NAMES mysqlcppconn)
if (LIBMYSQLCPPCONN_LIBRARY)
    message("MySQL C++ Connector library found")
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

# Hitta och länka mot SDL3
find_library(LIBSDL3_LIBRARY NAMES SDL3)
if (LIBSDL3_LIBRARY)
    message("SDL3 library found")
    target_link_libraries(GoobySoft PRIVATE ${LIBSDL3_LIBRARY})
else()
    message(FATAL_ERROR "SDL3 library not found")
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

# Hitta och länka mot OpenCV
find_package(OpenCV REQUIRED)
if (OpenCV_FOUND)
    message("OpenCV library found")
    target_link_libraries(GoobySoft PRIVATE ${OpenCV_LIBS})
else()
    message(FATAL_ERROR "OpenCV library not found")
endif()


# Hitta och länka mot CSerialPort
set(CSerialPortRootPath "${CMAKE_CURRENT_SOURCE_DIR}/../CSerialPort/")
include_directories(${CSerialPortRootPath}/include)
list(APPEND CSerialPortSourceFiles ${CSerialPortRootPath}/src/SerialPort.cpp ${CSerialPortRootPath}/src/SerialPortBase.cpp ${CSerialPortRootPath}/src/SerialPortInfo.cpp ${CSerialPortRootPath}/src/SerialPortInfoBase.cpp)
if(WIN32)
    message("CSerialPort library not found for Win32")
    list(APPEND CSerialPortSourceFiles ${CSerialPortRootPath}/src/SerialPortInfoWinBase.cpp ${CSerialPortRootPath}/src/SerialPortWinBase.cpp)
elseif(UNIX)
    message("CSerialPort library not found for Unix")
    list(APPEND CSerialPortSourceFiles ${CSerialPortRootPath}/src/SerialPortInfoUnixBase.cpp ${CSerialPortRootPath}/src/SerialPortUnixBase.cpp)
endif()