# GoobySoft

This is a measuring and controlling software. The purpose of this software is to provide an open source measurement software and control software that is easy to maintain and easy to use. 

This software is a long time development project. Here I upload the communication protocols such as `Modbus`, `CANopen`, `SAE-J1939` etc. so they can be used to communicate with different industrial devices.

The project is written in C++20 and it's using `ImGui` as graphics library.

# Supported devices

 - ADL400 DIN Rail Energy Meter
 - Beijer Electronics Performance Inverter P2 (Comming soon)
 - [STM32-PLC](https://github.com/DanielMartensson/STM32-PLC) (Comming soon)

# Pictures

# How to install

## Windows

 1. Install `Visual Studio Community`
 2. Install `vcpkg`
 3. Configure `vcpkg` with `Visual Studio Community`
 4. Install the following packages: `glfw3`, `opengl`, `mysql-connector-cpp`, `libmodbus`, `boost-asio`, `boost-date-time`, `boost-filesystem`

## Ubuntu/Debian

 1. Install a C++ compiler `sudo apt install build-essential`
 2. Install CMake `sudo apt-get install cmake`
 3. Install GLFW dev files `sudo apt-get install libglfw3-dev`
 4. Install OpenGL dev files `sudo apt-get install libopengl-dev`
 5. Install Libmodbus dev files `sudo apt-get install libmodbus-dev`
 6. Install Boost dev files `sudo apt-get install libbost-all-dev`
 7. Configure MySQL APT Repository 
 8. Update APT `sudo apt-get update`
 9. Install MySQL C++ Connector dev files `sudo apt-get install libmysqlcppconn-dev`
 10. Run CMake `cmake -B build .`
 11. Compile `cmake --build build`    

USB connection

![a](https://github.com/DanielMartensson/GoobySoft/blob/main/Pictures/USB.png?raw=true)

Database connection

![a](https://github.com/DanielMartensson/GoobySoft/blob/main/Pictures/Database.png?raw=true)

View measurements

![a](https://github.com/DanielMartensson/GoobySoft/blob/main/Pictures/ViewMeasurement.png?raw=true)

Measuring process

![a](https://github.com/DanielMartensson/GoobySoft/blob/main/Pictures/Measure.png?raw=true)

Configure measurement

![a](https://github.com/DanielMartensson/GoobySoft/blob/main/Pictures/ConfigureMeasurement.png?raw=true)

File dialog

![a](https://github.com/DanielMartensson/GoobySoft/blob/main/Pictures/FileDialog.png?raw=true)