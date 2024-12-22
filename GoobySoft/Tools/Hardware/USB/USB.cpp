#include "USB.h"
#include "../../Tools.h"

size_t Tools_Hardware_USB_getPortIndex(const char port[]) {
	size_t indexOfNumber = std::string(port).find_first_of("0123456789");
	size_t index = size_t(port[indexOfNumber]) - 48U;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	index--; // In Windows, then COM ports start from 1 e.g COM1
#endif
	if (index >= MAX_USB_PORTS) {
		index = MAX_USB_PORTS - 1U;
	}
	return index;
}

USB_STATUS Tools_Hardware_USB_isConnected(const char port[]) {
	return Tools_Hardware_USB_Protocols_CDC_isConnected(port) ? USB_STATUS_CONNECTED : USB_STATUS_NOT_CONNECTED;
}

USB_STATUS Tools_Hardware_USB_closeConnection(const char port[]) {
	return Tools_Hardware_USB_Protocols_CDC_closeConnection(port) ? USB_STATUS_DISCONNECTED : USB_STATUS_NOT_CONNECTED;
}

USB_STATUS Tools_Hardware_USB_openConnection(const char port[], const unsigned int baudrate, const unsigned int dataBits, const std::string& flowControl, const std::string& stopBits, const std::string& parity, const std::string& protocols) {
	// Check if we are already connected 
	if (Tools_Hardware_USB_isConnected(port) == USB_STATUS_CONNECTED) {
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

	// Get the selection of which protocols is being used
	return Tools_Hardware_USB_Protocols_CDC_openConnection(port, baudrate, dataBits, flowControlCDC, stopBitsCDC, parityCDC) ? USB_STATUS_CONNECTED : USB_STATUS_FAIL;
}

std::vector<std::string> Tools_Hardware_USB_getAllPorts() {
	return Tools_Hardware_USB_Protocols_CDC_getAllPorts(); 	// CDC is using Boost Asio and can locate all existing ports
}

std::string Tools_Hardware_USB_getConnectedPorts() {
	return Tools_Hardware_USB_Protocols_CDC_getPortsOfConnectedDevices();
}

std::string Tools_Hardware_USB_getProtocolFromPort(const char port[]) {
	size_t index = Tools_Hardware_USB_getPortIndex(port);
	return PROTOCOL_STRING[Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[index].protocolIndex];
}