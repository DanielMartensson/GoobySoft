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

 1. Download and Configure [Oracle APT Repository](https://dev.mysql.com/downloads/repo/apt/)
 2. Update APT `sudo apt-get update`
 3. Install software `sudo apt-get install build-essential, cmake, libglfw3-dev, libopengl-dev, libmodbus-dev, libboost-all-dev, libmysqlcppconn-dev`
 4. Download `GoobySoft` repository and extract the `.zip` file.
 5. Make sure you are standing inside the `GoobySoft` folder.
 5. Run CMake `cmake -B build .`
 6. Compile `cmake --build build` 
 7. Go to the `build` folder inside `GoobySoft` and run `./GoobySoft`  

## Raspberry Pi
 1. Install software `sudo apt-get install build-essential, cmake, libglfw3-dev, libopengl-dev, libmodbus-dev, libboost-all-dev`
 2. Download the `Linux - Generic (glibc <version>) (ARM, 64-bit), Compressed TAR Archive` from [Oracle Connectors](https://dev.mysql.com/downloads/connector/cpp/)
 3. Extract the tar archive and stand inside the extracted folder. You will now see the folders `include` and `lib`.
 4. Run `sudo cp -r include/* /usr/include/mysql-cppconn-8/`
 5. Run `sudo cp -r lib/* /lib/aarch64-linux-gnu/`
. Download `GoobySoft` repository and extract the `.zip` file.
 6. Make sure you are standing inside the `GoobySoft` folder.
 7. Run CMake `cmake -B build .`
 8. Compile `cmake --build build` 
 9. Go to the `build` folder inside `GoobySoft` and run `./GoobySoft`  

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