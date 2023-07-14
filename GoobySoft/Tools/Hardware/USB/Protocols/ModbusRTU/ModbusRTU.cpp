#include "ModbusRTU.h"
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <modbus.h>
#else
#include <modbus/modbus.h>
#endif
#include <boost/asio.hpp>
#include <map>

static std::map<std::string, std::pair<modbus_t*, bool>> devicesModbus;

static bool modbusDeviceExist(const char port[]) {
	return devicesModbus.find(port) != devicesModbus.end() ? true : false;
}

static modbus_t* getDevice(const char port[]) {
	return devicesModbus.at(port).first;
}

bool Tools_Hardware_USB_Protocols_ModbusRTU_isConnected(const char port[]) {
	return modbusDeviceExist(port);
}

bool Tools_Hardware_USB_Protocols_ModbusRTU_readRegister(const char port[], const unsigned int registerAddress, const unsigned int numberOfRegisters, uint16_t registers[]) {
	if (Tools_Hardware_USB_Protocols_ModbusRTU_isConnected(port)) {
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

bool Tools_Hardware_USB_Protocols_ModbusRTU_closeConnection(const char port[]) {
	if (Tools_Hardware_USB_Protocols_ModbusRTU_isConnected(port)) {
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

bool Tools_Hardware_USB_Protocols_ModbusRTU_openConnection(const char port[], const unsigned int baudrate, const char parity, const unsigned int dataBit, const unsigned int stopBits) {
	modbus_t* devices = modbus_new_rtu(port, baudrate, parity, dataBit, stopBits);
	if (modbus_connect(devices) == -1) {
#if(defined(_GOOBYSOFT_DEBUG))
		std::printf("Modbus RTU connection failed: %s\n", modbus_strerror(errno));
#endif
		modbus_free(devices);
		return false;
	}
	else {
		devicesModbus[port] = { devices, false };
		return true;
	}
}

bool Tools_Hardware_USB_Protocols_ModbusRTU_openConnection(const char ipAddress[], const unsigned int port) {
	// This is a special case because I cannot include <modbus.h> inside a header file with Visual Studio Community 2022. Getting C2059 error
	modbus_t* devices = modbus_new_tcp(ipAddress, port);
	if (modbus_connect(devices) == -1) {
#if(defined(_GOOBYSOFT_DEBUG))
		std::printf("Modbus TCP connection failed: %s\n", modbus_strerror(errno));
#endif
		modbus_free(devices);
		return false;
	}
	else {
		std::string ipAddressPort = std::string(ipAddress) + ":" + std::to_string(port);
		devicesModbus[ipAddressPort] = { devices, true };
		return true;
	}
}

bool Tools_Hardware_USB_Protocols_ModbusRTU_setSlaveAddress(const char port[], const unsigned int slaveAddress) {
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

std::string Tools_Hardware_USB_Protocols_ModbusRTU_getPortsOfConnectedDevices(bool isTCP) {
	std::string ports;
	size_t maxDevicesModbus = devicesModbus.size();
	size_t count = 0;
	for (auto const& [port, devices] : devicesModbus) {
		if (devices.second == isTCP) {
			ports += port.c_str();
			if (count < maxDevicesModbus - 1) {
				ports += '\0';
			}
			count++;
		}
	}
	return ports;
}