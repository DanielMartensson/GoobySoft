# GoobySoft

This is a measuring and controlling software. The purpose of this software is to provide an open source measurement software and control software that is easy to maintain and easy to use. The unique thing with this project is that this project is written so anybody can implement their own `device`. 

# Features

* Open SAE J1939/Modbus RTU/Modbus TCP/USB support
* Measure digital/analog inputs
* Control output actuators
* Easy to add own `device` by using `callback` functions
* Database connection
* Real time data acquisition
* Multifunctional plots
* CAN Traffic reader
* Multiple connections to multiple devices at the same time during data acquisition
* Latest modern C++ standard

# Supported devices

 - ADL400 DIN Rail Energy Meter
 - Uponor X-148 Home Automation System
 - [STM32-PLC](https://github.com/DanielMartensson/STM32-PLC)
 - [Weller to JBC converter](https://github.com/DanielMartensson/Weller-to-JBC-converter)

# How to add a new `device`

Assume that you have for example a `Modbus` or a special USB dongle that can communicate with an `input/output device`, and you want `GoobySoft` to read, write and store measurement data and also have a control connection to that `device`. All you need to do, is to work inside `Devices` folder of this project.

1. Begin first to add your new `protocol` and new `device` here. Notice the name `Tools_Communications_Devices_createDevices()`. The project is structured so each folder name begins with a capital letter and subfolders path are displayed with `_` and functions begins with lower case letters. So to find `Tools_Communications_Devices_createDevices()`, head over to `Tools/Communications/Devices` and open the file `Devices.cpp`. Each folder have the same folder name as the header and source file. So inside folder `Devices`, it exists `Devices.cpp` and `Devices.h`.
```cpp
void Tools_Communications_Devices_createDevices() {
	// Get the parameter holder
	Protocol* protocols = Tools_Hardware_ParameterStore_getParameterHolder()->protocols;

	// Reset all
	for (int i = 0; i < MAX_PROTOCOLS; i++) {
		protocols[i].isProtocolUsed = false;
	}

	// Create devices for protocols 
	createProtocolTool(&protocols[0], PROTOCOL_STRING[USB_PROTOCOL_ENUM_MODBUS_RTU], 1); // Modbus RTU, 1 device
	createProtocolTool(&protocols[1], PROTOCOL_STRING[USB_PROTOCOL_ENUM_CDC], 1); // CDC (Communication Device Class a.k.a own USB protocol), 1 device
	// Add new protocol here...

	// Create devices
	createDeviceTool(&protocols[0].devices[0], "ADL400", Tools_Communications_Devices_ADL400_getFunctionValues, Tools_Communications_Devices_ADL400_getTableColumnIDs, Tools_Communications_Devices_ADL400_getInput, Tools_Communications_Devices_ADL400_setOutput, Tools_Communications_Devices_ADL400_getColumnFunction);
	createDeviceTool(&protocols[1].devices[0], "STM32 PLC", Tools_Communications_Devices_STM32PLC_getFunctionValues, Tools_Communications_Devices_STM32PLC_getTableColumnIDs, Tools_Communications_Devices_STM32PLC_getInput, Tools_Communications_Devices_STM32PLC_setOutput, Tools_Communications_Devices_STM32PLC_getColumnFunction);
	// Add new device here...
}
```

2. Create the `getFunctionValues()` callback. This function should return a string of function values with `\0` as null termination e.g `Read Input A\0Read Input B\0Write Output C\0`. The reason for that is that `ImGui::Combo` box want an argument that contains a `const char*` that null terminations
```cpp
std::string Tools_Communications_Devices_<NAME_OF_YOUR_DEVICE>_getFunctionValues(){
	std::string functionNames;
	functionNames += "Read Input A";
	functionNames += '\0';
	functionNames += "Read Input B";
	functionNames += '\0';
	functionNames += "Write Output C";
	functionNames += '\0';
	return functionNames;
}
```

3. Create the `getTableColumnsID()` callback. Here you can determine the name of your column when you are going to configure your e.g measurement `device` or `CAN-bus device`. Here are some examples below. You don't need to use them all, but some of them are mandatory. 
```cpp
std::vector<TableColumnID> Tools_Communications_Devices_<NAME_OF_YOUR_DEVICE>_getTableColumnIDs() {
	/* 
         * This can:
         * Measure analog/digital inputs, control analog outputs, control analog outputs via e.g CAN-bus field, measure analog/digital inputs via e.g CAN-bus field
         */
	// Only one column definition is allowed.
	std::vector<TableColumnID> tableColumnIDs;
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Port", COLUMN_DEFINITION_PORT)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Function", COLUMN_DEFINITION_FUNCTION)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("CAN address", COLUMN_DEFINITION_ADDRESS));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Min value raw", COLUMN_DEFINITION_MIN_VALUE_RAW));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Max value raw", COLUMN_DEFINITION_MAX_VALUE_RAW));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Min value real", COLUMN_DEFINITION_MIN_VALUE_REAL));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Max value real", COLUMN_DEFINITION_MAX_VALUE_REAL));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Display name", COLUMN_DEFINITION_DISPLAY_NAME)); // Mandatory field
	return tableColumnIDs;

	/* 
         * This can:
         * Measure analog/digital inputs, control analog outputs
         */
	// Only one column definition is allowed.
	std::vector<TableColumnID> tableColumnIDs;
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Port", COLUMN_DEFINITION_PORT)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Function", COLUMN_DEFINITION_FUNCTION)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Min value raw", COLUMN_DEFINITION_MIN_VALUE_RAW));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Max value raw", COLUMN_DEFINITION_MAX_VALUE_RAW));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Min value real", COLUMN_DEFINITION_MIN_VALUE_REAL));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Max value real", COLUMN_DEFINITION_MAX_VALUE_REAL));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Display name", COLUMN_DEFINITION_DISPLAY_NAME)); // Mandatory field
	return tableColumnIDs;

	/* 
         * This can:
         * Control analog outputs
         */
	// Only one column definition is allowed.
	std::vector<TableColumnID> tableColumnIDs;
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Port", COLUMN_DEFINITION_PORT)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Function", COLUMN_DEFINITION_FUNCTION)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Min value raw", COLUMN_DEFINITION_MIN_VALUE_RAW));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Max value raw", COLUMN_DEFINITION_MAX_VALUE_RAW));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Display name", COLUMN_DEFINITION_DISPLAY_NAME)); // Mandatory field
	return tableColumnIDs;

	/* 
         * This can:
         * Measure analog/digital inputs via e.g CAN-bus field
         */
	// Only one column definition is allowed.
	std::vector<TableColumnID> tableColumnIDs;
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Port", COLUMN_DEFINITION_PORT)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Function", COLUMN_DEFINITION_FUNCTION)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("CAN/Modbus address", COLUMN_DEFINITION_ADDRESS));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Display name", COLUMN_DEFINITION_DISPLAY_NAME)); // Mandatory field
	return tableColumnIDs;

	/* 
         * This can:
         * Control analog outputs via e.g CAN-bus field, measure analog/digital inputs via e.g CAN-bus field
         */
	// Only one column definition is allowed.
	std::vector<TableColumnID> tableColumnIDs;
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Port", COLUMN_DEFINITION_PORT)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Function", COLUMN_DEFINITION_FUNCTION)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("CAN/Modbus address", COLUMN_DEFINITION_ADDRESS));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Min value raw", COLUMN_DEFINITION_MIN_VALUE_RAW));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Max value raw", COLUMN_DEFINITION_MAX_VALUE_RAW));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Display name", COLUMN_DEFINITION_DISPLAY_NAME)); // Mandatory field
	return tableColumnIDs;

}
```

If you have this setup above, then your configuration table is going to look like this. Depending on which function you are selecting, some input fields are hidden. The `COLUMN_DEFINITION enum` can be found in `Parameters.h` file

![a](https://github.com/DanielMartensson/GoobySoft/blob/main/Pictures/ConfigureMeasurement.png?raw=true)


4. Create the `getInput()` callback. This function want to have three arguments. A `C-string` port that describe the `USB` port, or it can also be the `Ip Address` if `Modbus TCP` is used. Next argument is the `functionValueIndex`. That index value corresponds to the index of `getFunctionValues()` callback. What `getInput()` does, is that it's reading the measurements of a device and return it back

```cpp
float Tools_Communications_Devices_<NAME_OF_YOUR_DEVICE>_getInput(const char port[], int functionValueIndex, int address) {
	/* These must follow the same linear pattern as getFunctionValues() */
	setSlaveAddress(port, address); // set slave address if you are using e.g Modbus etc. If not, then you can remove this line
	switch (functionValueIndex) {
	case IO_READ_INPUT_A:
		return functionThatReadsInputA(port);
	case IO_READ_INPUT_B:
		return functionThatReadsInputB(port);
	default:
		return -1.0f;
	}
}
```

5. Create the `IO enum`. This enum is going to serve the argument `functionValueIndex` together with a `switch`-statement.
```cpp
/* These must follow the same linear pattern as getFunctionValues() */
typedef enum {
	IO_READ_INPUT_A,
	IO_READ_INPUT_B,
	IO_WRITE_OUTPUT_C
}IO;
```

6. Create the `setOutput()` callback. Four arguments, same as `getInput()` callback, but this one have an integer `value` that `GoobySoft` is sending to the `device` for e.g `PWM control`.
```cpp
bool Tools_Communications_Devices_<NAME_OF_YOUR_DEVICE>_setOutput(const char port[], int functionValueIndex, int address, int value) {
	/* These must follow the same linear pattern as getFunctionValues() */
	switch (functionValueIndex) {
	case IO_WRITE_OUTPUT_C:
		// If you are using a device who wants an address, you might consider to include `address` argument inside the funftion
		return funtionThatWritesOutputC(port, value); 
	default:
		return false; // Fail
	}
}
```

7. Create the `getColumnFunction()` callback. Here you are going to select which `IO` that has a specific purpose. Some functions might be a `CAN-bus` output or input or some functions might be a pure analog input with 16-bit ADC e.g `STM32`. The `COLUMN_FUNCTION enum` can be found in `Parameters.h` file
```cpp
COLUMN_FUNCTION Tools_Communications_Devices_<NAME_OF_YOUR_DEVICE>_getColumnFunction(int functionValueIndex) {
	/* These must follow the same linear pattern as getFunctionValues() */
	switch (functionValueIndex) {
	case IO_READ_INPUT_A:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS; // E.g STM32 with CAN-bus sensor
	case IO_READ_INPUT_B:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG; // E.g Arduino ADC
	case IO_WRITE_OUTPUT_C:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR_ADDRESS; // E.g PWM CAN-bus unit
	default:
		return COLUMN_FUNCTION_HOLD_DATA; // Just hold data
	}
}
```

It's very important to select right `COLUMN_FUNCTION` for a specific `IO` index. The `getColumnFunction()` callback determine how your configuration window will look like and if your sensor is going to be calibrated or not.

8. Now when you have made your protocol for your `device`, it's time to connect them to `GoobySoft`

```cpp
/* 
 * One protocol can contains multiple devices.
 * PROTOCOL_STRING[USB_PROTOCOL_ENUM_CDC] stands for USB communications device class e.g regular USB communication
 * Here I say that `CDC` can hold 10 devices.
 */
createProtocolTool(&protocols[1], PROTOCOL_STRING[USB_PROTOCOL_ENUM_CDC], 10); 


/* Add your device to that protocol CDC. Here I say that my device is CDC and the device is at index 0 */
createDeviceTool(&protocols[1].devices[0], "<NAME_OF_YOUR_DEVICE>", 
Tools_Communications_Devices_<NAME_OF_YOUR_DEVICE>_getFunctionValues, 
Tools_Communications_Devices_<NAME_OF_YOUR_DEVICE>_getTableColumnIDs, 
Tools_Communications_Devices_<NAME_OF_YOUR_DEVICE>_getInput, 
Tools_Communications_Devices_<NAME_OF_YOUR_DEVICE>_setOutput, 
Tools_Communications_Devices_<NAME_OF_YOUR_DEVICE>_getColumnFunction);

```

Now you are done! For examples, head over to [Devices](https://github.com/DanielMartensson/GoobySoft/tree/main/GoobySoft/Tools/Communications/Devices) folder.

If you got some issues with the combo boxes e.g for `Functions` or want to add more devices, check out the configuration inside `Parameters.h` file

```cpp
#define MAX_PROTOCOLS 5			// How many protocols can be used, CDC, Modbus RTU, Modbus TCP etc..
#define MAX_DEVICES 10			// How many devices per protocol
#define MAX_ROWS 10			// How many rows per device inside configuration window
#define MAX_COLUMNS 10			// Max columns for each device inside configuration column
#define MAX_USB_PORTS 10		// Max USB ports that can be connected at the same time
#define MAX_C_STRING_LEN 30		// Max char* length for e.g port, device name etc.
#define MAX_C_STRING_EXTRA_LEN 1024	// Max char* length for function values
#define MAX_DATA_MEASUREMENT_PLOT 1024	// Max plot length for real time measuring
```


# How to install

The folders `Debug` and `Release` contains prebuilt 32-bit Windows binaries for `GoobySoft`.
The folders `x64/Debug` and `x64/Release` contains prebuilt 64-bit Windows binaries for `GoobySoft`.

## Windows - Compile from source

 1. Install `Visual Studio Community`
 2. Install `Vcpkg`
 3. Install `Intel Math Kernel Library` manually from Intel.
 4. Configure `vcpkg` with `Visual Studio Community`
 5. Install the following vcpkg packages: `sdl3`, `opengl`, `opencv`, `mysql-connector-cpp`, `cserialport`, `boost-date-time`, `boost-filesystem`, `boost-chrono`
 6. Run the `GoobySoft.sln` file with `Visual Studio Community` and press the compile button.

## Ubuntu/Debian - Compile from source

 1. Download and Configure [Oracle APT Repository](https://dev.mysql.com/downloads/repo/apt/)
 2. Update APT `sudo apt-get update`
 3. Install software `sudo apt-get install build-essential cmake libsdl3-dev libopencv-dev intel-oneapi-mkl libopengl-dev libboost-all-dev libmysqlcppconn-dev`
 4. Download `GoobySoft` repository and extract the `.zip` file.
 5. Make sure you are standing inside the `GoobySoft` folder.
 5. Run CMake `cmake -B build .`
 6. Compile `cmake --build build` 
 7. Go to the `build` folder inside `GoobySoft` and run `./GoobySoft`  

Or your could just run the script `installForUbuntuDebian.sh`

```bash
./installForUbuntuDebian.sh
```

## Raspberry Pi - Compile from source
 1. Install software `sudo apt-get install build-essential cmake libsdl3-dev libopengl-dev libboost-all-dev`
 2. Download the `Linux - Generic (glibc <version>) (ARM, 64-bit), Compressed TAR Archive` from [Oracle Connectors](https://dev.mysql.com/downloads/connector/cpp/)
 3. Extract the tar archive and stand inside the extracted folder. You will now see the folders `include` and `lib64`.
 4. Write `sudo mkdir /usr/include/mysql-cppconn-8/`
 5. Write `sudo cp -r include/* /usr/include/mysql-cppconn-8/`
 6. Write `sudo cp -r lib64/* /lib/aarch64-linux-gnu/`
 7. Download `GoobySoft` repository and extract the `.zip` file.
 8. Make sure you are standing inside the `GoobySoft` folder.
 9. Run CMake `cmake -B build .`
 10. Compile `cmake --build build` 
 11. Go to the `build` folder inside `GoobySoft` and run `./GoobySoft`  

Or your could just run the script `installForRaspberryPi.sh`

```bash
./installForRaspberryPi.sh
```

# Pictures

SAE J1939 Identifications

![a](https://github.com/DanielMartensson/GoobySoft/blob/main/Pictures/ConfigureSAEJ1939Identifications.png?raw=true)

USB connection

![a](https://github.com/DanielMartensson/GoobySoft/blob/main/Pictures/USB.png?raw=true)

Database connection

![a](https://github.com/DanielMartensson/GoobySoft/blob/main/Pictures/Database.png?raw=true)

View measurements

![a](https://github.com/DanielMartensson/GoobySoft/blob/main/Pictures/ViewMeasurement.png?raw=true)

Measuring process

![a](https://github.com/DanielMartensson/GoobySoft/blob/main/Pictures/Measure.png?raw=true)


File dialog

![a](https://github.com/DanielMartensson/GoobySoft/blob/main/Pictures/FileDialog.png?raw=true)