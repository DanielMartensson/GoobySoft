cmake_minimum_required(VERSION 3.28.3)
project(GoobySoft)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Release")

# Inkludera externa bibliotek från Libraries-mappen
add_subdirectory(GoobySoft/Tools/Software/Libraries/OpenSAEJ1939/Open-SAE-J1939)
add_subdirectory(GoobySoft/Tools/Software/Libraries/CControl)

# Hämta alla .cpp-filer i GoobySoft mappen
file(GLOB_RECURSE ALL_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/GoobySoft/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/GoobySoft/*.c")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/GoobySoft/*.h")

# Exkludera main.c från CControl och OpenSAEJ1939 om de hamnat med (fast du har cpp-filer, men bra att ha som exempel)
set(MAIN_SRC_CControl "${CMAKE_CURRENT_SOURCE_DIR}/GoobySoft/Tools/Software/Libraries/CControl/src/Main.c")
set(MAIN_SRC_Open_SAE_J1939 "${CMAKE_CURRENT_SOURCE_DIR}/GoobySoft/Tools/Software/Libraries/OpenSAEJ1939/Open-SAE-J1939/Src/Main.c")

list(REMOVE_ITEM ALL_SOURCE_FILES ${MAIN_SRC_CControl})
list(REMOVE_ITEM ALL_SOURCE_FILES ${MAIN_SRC_Open_SAE_J1939})

# Skapa körbar fil
add_executable(GoobySoftRun ${ALL_SOURCE_FILES})

# Inkludera egna headers (kan justeras efter behov)
target_include_directories(GoobySoftRun PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/GoobySoft")

# Länka mot MySQL C++ Connector
find_package(mysql-concpp CONFIG REQUIRED)
if(mysql-concpp_FOUND)
    # Lägg till sökvägen där libmysqlcppconnx-static.a ligger
    if(UNIX)
        message(STATUS "MySQL C++ Connector library found - Unix")
        target_link_directories(GoobySoftRun PRIVATE "${CMAKE_SOURCE_DIR}/../vcpkg/installed/x64-linux/lib")
        target_link_libraries(GoobySoftRun PRIVATE mysqlcppconnx-static)
    else()
    
    endif()
else()
    message(FATAL_ERROR "MySQL C++ Connector library not found")
endif()

# Länka mot Resolv
if(UNIX)
    find_library(RESOLV_LIB resolv)
    if (RESOLV_LIB)
        message(STATUS "libresolv found: ${RESOLV_LIB}")
        target_link_libraries(GoobySoftRun PRIVATE ${RESOLV_LIB})
    else()
        message(FATAL_ERROR "libresolv not found")
    endif()
endif()

# Länka mot libdl
if(UNIX)
    find_library(LIBDL_LIBRARY NAMES dl)
    if (LIBDL_LIBRARY)
        message(STATUS "libdl library found")
        target_link_libraries(GoobySoftRun PRIVATE ${LIBDL_LIBRARY})
    else()
        message(FATAL_ERROR "libdl library not found")
    endif()
endif()

# Länka mot OpenSSL
find_package(OpenSSL REQUIRED)
if(OpenSSL_FOUND)
    message(STATUS "OpenSSL found!")
    target_link_libraries(GoobySoftRun PRIVATE OpenSSL::SSL OpenSSL::Crypto)
else()
    message(FATAL_ERROR "OpenSSL not found!")
endif()

# Länka mot SDL3
find_library(LIBSDL3_LIBRARY NAMES SDL3)
if (LIBSDL3_LIBRARY)
    message(STATUS "SDL3 library found")
    target_link_libraries(GoobySoftRun PRIVATE ${LIBSDL3_LIBRARY})
else()
    message(FATAL_ERROR "SDL3 library not found")
endif()

# Länka mot Threads
if(UNIX)
    find_package(Threads REQUIRED)
    if(Threads_FOUND)
        message(STATUS "Thread library found")
        target_link_libraries(GoobySoftRun PRIVATE Threads::Threads)
    else()
        message(FATAL_ERROR "Thread library not found")
    endif()
else()
    find_package(PThreads4W REQUIRED)
    if(PThreads4W_FOUND)
        message(STATUS "PThread library found")
        target_link_libraries(GoobySoftRun PRIVATE PThreads4W::PThreads4W)
    else()
        message(FATAL_ERROR "PThread library not found")
    endif()
endif()

# Länka mot Boost filesystem
find_package(Boost CONFIG REQUIRED COMPONENTS filesystem)
if (Boost_FOUND)
    message(STATUS "Boost filesystem library found")
    target_link_libraries(GoobySoftRun PRIVATE Boost::filesystem)
else()
    message(FATAL_ERROR "Boost filesystem library not found")
endif()

# Länka mot Boost system
find_package(Boost CONFIG REQUIRED COMPONENTS system)
if (Boost_FOUND)
    message(STATUS "Boost system library found")
    target_link_libraries(GoobySoftRun PRIVATE Boost::system)
else()
    message(FATAL_ERROR "Boost system library not found")
endif()

# Länka mot Boost date_time
find_package(Boost CONFIG REQUIRED COMPONENTS date_time)
if (Boost_FOUND)
    message(STATUS "Boost date time library found")
    target_link_libraries(GoobySoftRun PRIVATE Boost::date_time)
else()
    message(FATAL_ERROR "Boost date time library not found")
endif()

# Länka mot OpenGL
find_package(OpenGL REQUIRED)
if (OpenGL_FOUND)
    message(STATUS "OpenGL library found")
    target_link_libraries(GoobySoftRun PRIVATE OpenGL::GL)
    target_include_directories(GoobySoftRun PRIVATE ${OPENGL_INCLUDE_DIR})
else()
    message(FATAL_ERROR "OpenGL library not found")
endif()

# Länka mot OpenCV
find_package(OpenCV REQUIRED)
if (OpenCV_FOUND)
    message(STATUS "OpenCV library found")
    target_link_libraries(GoobySoftRun PRIVATE ${OpenCV_LIBS})
else()
    message(FATAL_ERROR "OpenCV library not found")
endif()

# Länka mot CSerialPort
find_package(CSerialPort REQUIRED)
if (CSerialPort_FOUND)
    message(STATUS "CSerialPort library found")
    target_link_libraries(GoobySoftRun PRIVATE cserialport)
else()
    message(FATAL_ERROR "CSerialPort library not found")
endif()

# Länka mot zstd
find_package(zstd CONFIG REQUIRED)
if (zstd_FOUND)
    message(STATUS "zstd library found")
    target_link_libraries(GoobySoftRun PRIVATE zstd::libzstd)
else()
    message(FATAL_ERROR "zstd library not found")
endif()

# Länka mot lz4
find_package(lz4 CONFIG REQUIRED)
if (lz4_FOUND)
    message(STATUS "lz4 library found")
    target_link_libraries(GoobySoftRun PRIVATE lz4::lz4)
else()
    message(FATAL_ERROR "lz4 library not found")
endif()