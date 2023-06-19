#include "USB.h"
#include "../../Tools.h"

size_t Tools_Hardware_USB_getPortIndex(const char port[]) {
	size_t indexOfNumber = std::string(port).find_first_of("0123456789");
	size_t index = size_t(port[indexOfNumber]) - 48U;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	index--; // In Windows, then COM ports start from 1 e.g COM1
#endif
	if (index >= MAX_USB_DEVICES) {
		index = MAX_USB_DEVICES - 1U;
	}
	return index;
}

USB_STATUS Tools_Hardware_USB_isConnected(const char port[], const std::string& protocol) {
	int index = Tools_Software_Algorithms_findIndexOf(USB_PROTOCOL_STRING, protocol);
	switch (index) {
	case USB_PROTOCOL_ENUM_MODBUS:
		return Tools_Hardware_USB_Protocols_Modbus_isConnected(port) ? USB_STATUS_CONNECTED : USB_STATUS_NOT_CONNECTED;
	case USB_PROTOCOL_ENUM_CDC:
		return Tools_Hardware_USB_Protocols_CDC_isConnected(port) ? USB_STATUS_CONNECTED : USB_STATUS_NOT_CONNECTED;
	}
	return USB_STATUS_NO_PROTOCOL;
}

USB_STATUS Tools_Hardware_USB_closeConnection(const char port[], const std::string& protocol) {
	int index = Tools_Software_Algorithms_findIndexOf(USB_PROTOCOL_STRING, protocol);
	switch (index) {
	case USB_PROTOCOL_ENUM_MODBUS:
		return Tools_Hardware_USB_Protocols_Modbus_closeConnection(port) ? USB_STATUS_DISCONNECTED : USB_STATUS_NOT_CONNECTED;
	case USB_PROTOCOL_ENUM_CDC:
		return Tools_Hardware_USB_Protocols_CDC_closeConnection(port) ? USB_STATUS_DISCONNECTED : USB_STATUS_NOT_CONNECTED;
	}
	return USB_STATUS_NO_PROTOCOL;
}

USB_STATUS Tools_Hardware_USB_openConnection(const char port[], const unsigned int baudrate, const unsigned int dataBits, const std::string& flowControl, const std::string& stopBits, const std::string& parity, const std::string& protocol) {
	// Check if we are already connected 
	if (Tools_Hardware_USB_isConnected(port, protocol) == USB_STATUS_CONNECTED) {
		return USB_STATUS_CONNECTED;
	}

	// Translate
	int index = Tools_Software_Algorithms_findIndexOf(USB_PARITY_STRING, parity);
	int parityCDC = index;
	char parityModbus;
	switch (index) {
	case USB_PARITY_ENUM_NONE:
		parityModbus = 'N';
		break;
	case USB_PARITY_ENUM_EVEN:
		parityModbus = 'E';
		break;
	case USB_PARITY_ENUM_ODD:
		parityModbus = 'O';
		break;
	default:
		parityModbus = 'N';
		break;
	}
	int stopBitsModbus = Tools_Software_Algorithms_findIndexOf(USB_STOP_BITS_STRING, stopBits) + 1;
	int flowControlCDC = Tools_Software_Algorithms_findIndexOf(USB_CONTROL_FLOW_STRING, flowControl);
	int stopBitsCDC = Tools_Software_Algorithms_findIndexOf(USB_STOP_BITS_STRING, stopBits);

	// Get the selection of which protocol is being used
	index = Tools_Software_Algorithms_findIndexOf(USB_PROTOCOL_STRING, protocol);

	switch (index) {
	case USB_PROTOCOL_ENUM_MODBUS:
		return Tools_Hardware_USB_Protocols_Modbus_openConnection(port, baudrate, parityModbus, dataBits, stopBitsModbus) ? USB_STATUS_CONNECTED : USB_STATUS_FAIL;
	case USB_PROTOCOL_ENUM_CDC:
		return Tools_Hardware_USB_Protocols_CDC_openConnection(port, baudrate, dataBits, flowControlCDC, stopBitsCDC, parityCDC) ? USB_STATUS_CONNECTED : USB_STATUS_FAIL;
	}
	return USB_STATUS_NO_PROTOCOL;
}

std::vector<std::string> Tools_Hardware_USB_getAllPorts() {
	// CDC is using Boost Asio and can locate all existing ports
	return Tools_Hardware_USB_Protocols_CDC_getAllPorts();
}


std::vector<std::string> Tools_Hardware_USB_getConnectedPorts(const std::string& protocol) {
	// Recieve the port of the addDevice
	std::vector<std::string> ports;
	int index = Tools_Software_Algorithms_findIndexOf(USB_PROTOCOL_STRING, protocol);
	switch (index) {
	case USB_PROTOCOL_ENUM_MODBUS:
		ports = Tools_Hardware_USB_Protocols_Modbus_getPortsOfConnectedDevices();
		break;
	case USB_PROTOCOL_ENUM_CDC:
		ports = Tools_Hardware_USB_Protocols_CDC_getPortsOfConnectedDevices();
		break;
	}
	return ports;
}

std::string Tools_Hardware_USB_getProtocolFromPort(const char port[]) {
	size_t index = Tools_Hardware_USB_getPortIndex(port);
	return USB_PROTOCOL_STRING[Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[index].protocolIndex];
}