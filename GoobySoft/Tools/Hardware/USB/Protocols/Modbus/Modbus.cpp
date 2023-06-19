#include "Modbus.h"
#include <modbus.h>
#include <boost/asio.hpp>
#include <map>

std::map<std::string, modbus_t*> devices;

bool modbusDeviceExist(const char port[]) {
	return devices.find(port) != devices.end() ? true : false;
}

modbus_t* getDevice(const char port[]) {
	return devices.at(port);
}

bool Tools_Hardware_USB_Protocols_Modbus_isConnected(const char port[]) {
	if (!modbusDeviceExist(port)) {
		return false;
	}
	else {
		boost::asio::io_service io;
		try {
			// Open and see if it's bussy
			boost::asio::serial_port testPort(io, port);
			testPort.close(); // It seems that it's not used by anyone
			return false;
			
		}
		catch (const boost::system::system_error& ex) {
			if (boost::asio::error::no_permission == ex.code().value()) {
				return true; // Device is bussy
			}
		}
	}
	return false;
}

 bool Tools_Hardware_USB_Protocols_Modbus_readRegister(const char port[], const unsigned int registerAddress, const unsigned int numberOfRegisters, uint16_t registers[]) {
	if (Tools_Hardware_USB_Protocols_Modbus_isConnected(port)) {
		int registerCount = modbus_read_registers(getDevice(port), registerAddress, numberOfRegisters, registers);
		return registerCount == numberOfRegisters ? true : false;
	}
	else {
		return false;
	}
}

bool Tools_Hardware_USB_Protocols_Modbus_closeConnection(const char port[]) {
	if (Tools_Hardware_USB_Protocols_Modbus_isConnected(port)) {
		modbus_close(getDevice(port));
		modbus_free(getDevice(port));
		devices.erase(port);
		return true;
	}
	else {
		return false;
	}
}

bool Tools_Hardware_USB_Protocols_Modbus_openConnection(const char port[], const unsigned int baudrate, const char parity, const unsigned int dataBit, const unsigned int stopBits){
	// Connect to modbus
	modbus_t* addDevice = modbus_new_rtu(port, baudrate, parity, dataBit, stopBits);
	int status = modbus_connect(addDevice);
	if (status == -1) {
		modbus_free(addDevice);
		return false;
	}
	else {
		devices[port] = addDevice;
		return true;
	}
}

bool Tools_Hardware_USB_Protocols_Modbus_setSlaveAddress(const char port[], const unsigned int slaveAddress) {
	if (!modbusDeviceExist(port)) {
		return false;
	}
	int status = modbus_set_slave(getDevice(port), slaveAddress);
	return status == 0 ? true : false;
}

std::vector<std::string> Tools_Hardware_USB_Protocols_Modbus_getPortsOfConnectedDevices() {
	std::vector<std::string> ports;
	for (auto const& [port, addDevice] : devices) {
		ports.push_back(port);
	}
	return ports;
}