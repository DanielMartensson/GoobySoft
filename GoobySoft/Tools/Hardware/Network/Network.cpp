#include "Network.h"
#include "../../Tools.h"

NETWORK_STATUS Tools_Hardware_Network_isConnected(const char ipAddress[], const unsigned int port, const std::string& protocols) {
	int index = Tools_Software_Algorithms_findIndexOf(NETWORK_PROTOCOL_STRING, protocols);
	switch (index) {
	case NETWORK_PROTOCOL_ENUM_MODBUS_TCP:
		return Tools_Hardware_Network_Protocols_ModbusTCP_isConnected(ipAddress, port) ? NETWORK_STATUS_CONNECTED : NETWORK_STATUS_NOT_CONNECTED;
	}
	return NETWORK_STATUS_NO_PROTOCOL;
}

NETWORK_STATUS Tools_Hardware_Network_openConnection(const char ipAddress[], const unsigned int port, const std::string& protocols) {
	// Check if we are already connected 
	if (Tools_Hardware_Network_isConnected(ipAddress, port, protocols) == NETWORK_STATUS_CONNECTED) {
		return NETWORK_STATUS_CONNECTED;
	}

	// Get the selection of which protocols is being used
	int index = Tools_Software_Algorithms_findIndexOf(NETWORK_PROTOCOL_STRING, protocols);

	switch (index) {
	case NETWORK_PROTOCOL_ENUM_MODBUS_TCP:
		return Tools_Hardware_Network_Protocols_ModbusTCP_openConnection(ipAddress, port) ? NETWORK_STATUS_CONNECTED : NETWORK_STATUS_FAIL;
	}
	return NETWORK_STATUS_NO_PROTOCOL;
}

NETWORK_STATUS Tools_Hardware_Network_closeConnection(const char ipAddress[], const unsigned int port, const std::string& protocols) {
	int index = Tools_Software_Algorithms_findIndexOf(NETWORK_PROTOCOL_STRING, protocols);
	switch (index) {
	case NETWORK_PROTOCOL_ENUM_MODBUS_TCP:
		return Tools_Hardware_Network_Protocols_ModbusTCP_closeConnection(ipAddress, port) ? NETWORK_STATUS_DISCONNECTED : NETWORK_STATUS_NOT_CONNECTED;
	}
	return NETWORK_STATUS_NO_PROTOCOL;
}

std::string Tools_Hardware_Network_getConnectedIpAddresses(const std::string& protocols) {
	std::string ports;
	int index = Tools_Software_Algorithms_findIndexOf(NETWORK_PROTOCOL_STRING, protocols);
	switch (index) {
	case NETWORK_PROTOCOL_ENUM_MODBUS_TCP:
		return Tools_Hardware_USB_Protocols_ModbusRTU_getPortsOfConnectedDevices(true);
	}
	return ports;
}