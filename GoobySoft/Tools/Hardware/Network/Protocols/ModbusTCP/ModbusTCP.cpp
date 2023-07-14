#include "ModbusTCP.h"
#include "../../../Hardware.h"

bool Tools_Hardware_Network_Protocols_ModbusTCP_isConnected(const char ipAddress[], const unsigned int port) {
	std::string ipAddressPort = std::string(ipAddress) + ":" + std::to_string(port);
	return Tools_Hardware_USB_Protocols_ModbusRTU_isConnected(ipAddressPort.c_str());
}

bool Tools_Hardware_Network_Protocols_ModbusTCP_readRegister(const char ipAddress[], const unsigned int port, const unsigned int registerAddress, const unsigned int numberOfRegisters, uint16_t registers[]) {
	std::string ipAddressPort = std::string(ipAddress) + ":" + std::to_string(port);
	return Tools_Hardware_USB_Protocols_ModbusRTU_readRegister(ipAddressPort.c_str(), registerAddress, numberOfRegisters, registers);
}

bool Tools_Hardware_Network_Protocols_ModbusTCP_closeConnection(const char ipAddress[], const unsigned int port) {
	std::string ipAddressPort = std::string(ipAddress) + ":" + std::to_string(port);
	return Tools_Hardware_USB_Protocols_ModbusRTU_closeConnection(ipAddressPort.c_str());
}

bool Tools_Hardware_Network_Protocols_ModbusTCP_openConnection(const char ipAddress[], const unsigned int port) {
	return Tools_Hardware_USB_Protocols_ModbusRTU_openConnection(ipAddress, port);
}

bool Tools_Hardware_Network_Protocols_ModbusTCP_setSlaveAddress(const char ipAddress[], const unsigned int port, const unsigned int slaveAddress) {
	std::string ipAddressPort = std::string(ipAddress) + ":" + std::to_string(port);
	return Tools_Hardware_USB_Protocols_ModbusRTU_setSlaveAddress(ipAddressPort.c_str(), slaveAddress);
}

std::string Tools_Hardware_Network_Protocols_ModbusTCP_getPortsOfConnectedDevices() {
	return Tools_Hardware_USB_Protocols_ModbusRTU_getPortsOfConnectedDevices(true);
}