#include "Modbus.h"
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <modbus.h>
#else
#include <modbus/modbus.h>
#endif
#include <boost/asio.hpp>
#include <map>

static std::map<std::string, modbus_t*> devicesModbus;

static bool modbusDeviceExist(const char port[]) {
	return devicesModbus.find(port) != devicesModbus.end() ? true : false;
}

static modbus_t* getDevice(const char port[]) {
	return devicesModbus.at(port);
}

bool Tools_Hardware_USB_Protocols_Modbus_isConnected(const char port[]) {
	return modbusDeviceExist(port);
}

 bool Tools_Hardware_USB_Protocols_Modbus_readRegister(const char port[], const unsigned int registerAddress, const unsigned int numberOfRegisters, uint16_t registers[]) {
	 if (Tools_Hardware_USB_Protocols_Modbus_isConnected(port)) {
		if (modbus_read_registers(getDevice(port), registerAddress, numberOfRegisters, registers) == numberOfRegisters) {
			return true;
		}
#if(defined(_GOOBYSOFT_DEBUG))
		std::printf("Modbus read register failed: %s\n", modbus_strerror(errno));
#endif
		return false;
	}
	else {
		return false;
	}
}

bool Tools_Hardware_USB_Protocols_Modbus_closeConnection(const char port[]) {
	if (Tools_Hardware_USB_Protocols_Modbus_isConnected(port)) {
		modbus_close(getDevice(port));
		modbus_free(getDevice(port));
		devicesModbus.erase(port);
		return true;
	}
	else {
		if (modbusDeviceExist(port)) {
			modbus_free(getDevice(port));
			devicesModbus.erase(port);
		}
		return false;
	}
}

bool Tools_Hardware_USB_Protocols_Modbus_openConnection(const char port[], const unsigned int baudrate, const char parity, const unsigned int dataBit, const unsigned int stopBits){
	// Connect to modbus
	modbus_t* addDevice = modbus_new_rtu(port, baudrate, parity, dataBit, stopBits);
	if (modbus_connect(addDevice) == -1) {
#if(defined(_GOOBYSOFT_DEBUG))
		std::printf("Modbus connection failed: %s\n", modbus_strerror(errno));
#endif
		modbus_free(addDevice);
		return false;
	}
	else {
		devicesModbus[port] = addDevice;
		return true;
	}
}

bool Tools_Hardware_USB_Protocols_Modbus_setSlaveAddress(const char port[], const unsigned int slaveAddress) {
	if (!modbusDeviceExist(port)) {
		return false;
	}
	if (modbus_set_slave(getDevice(port), slaveAddress) == -1) {
#if(defined(_GOOBYSOFT_DEBUG))
		std::printf("Modbus set slave failed: %s\n", modbus_strerror(errno));
#endif
		return false;
	}
	return true;
}

std::vector<std::string> Tools_Hardware_USB_Protocols_Modbus_getPortsOfConnectedDevices() {
	std::vector<std::string> ports;
	for (auto const& [port, addDevice] : devicesModbus) {
		ports.push_back(port);
	}
	return ports;
}