#include "USB.h"
#include "../../Tools.h"
#include <CSerialPort/SerialPort.h>
#include <CSerialPort/SerialPortInfo.h>

// Map for all connections
static std::map<std::string, std::shared_ptr<itas109::CSerialPort>> connections;

bool Tools_Hardware_USB_checkIfExist(const char port[]) {
	return connections.find(port) != connections.end() ? true : false;
}

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
	if (Tools_Hardware_USB_checkIfExist(port)) {
		return connections[port]->isOpen() ? USB_STATUS_CONNECTED : USB_STATUS_NOT_CONNECTED;
	}else {
		return USB_STATUS_NOT_EXIST;
	}
}

USB_STATUS Tools_Hardware_USB_closeConnection(const char port[]) {
	if (Tools_Hardware_USB_checkIfExist(port)) {
		if (Tools_Hardware_USB_isConnected(port)) {
			connections[port]->close();
		}
		connections.erase(port);
		return USB_STATUS_DISCONNECTED;
	}else {
		return USB_STATUS_NOT_EXIST;
	}
}

USB_STATUS Tools_Hardware_USB_openConnection(const char port[], const unsigned int baudrate, const unsigned int dataBits, const std::string& flowControl, const std::string& stopBits, const std::string& parity, const std::string& protocols) {
	// Check if we are already connected 
	if (Tools_Hardware_USB_isConnected(port) == USB_STATUS_CONNECTED) {
		return USB_STATUS_CONNECTED;
	}

	// Translate databits
	itas109::DataBits CSerialPortDataBits;
	switch (dataBits) {
	case 5:
		CSerialPortDataBits = itas109::DataBits5;
		break;
	case 6:
		CSerialPortDataBits = itas109::DataBits6;
		break;
	case 7:
		CSerialPortDataBits = itas109::DataBits7;
		break;
	case 8:
		CSerialPortDataBits = itas109::DataBits8;
		break;
	default:
		CSerialPortDataBits = itas109::DataBits8;
		break;
	}

	// Translate stopbits
	itas109::StopBits CSerialPortStopBits = itas109::StopBits::StopOne;
	if (stopBits == "One") {
		CSerialPortStopBits = itas109::StopBits::StopOne;
	}else if (stopBits == "Two") {
		CSerialPortStopBits = itas109::StopBits::StopTwo;
	}

	// Translate flow control
	itas109::FlowControl CSerialPortFlowControl = itas109::FlowControl::FlowNone;
	if (flowControl == "None") {
		CSerialPortFlowControl = itas109::FlowControl::FlowNone;
	}else if (flowControl == "Hardware") {
		CSerialPortFlowControl = itas109::FlowControl::FlowHardware;
	}else if (flowControl == "Software") {
		CSerialPortFlowControl = itas109::FlowControl::FlowSoftware;
	}

	// Translate parity
	itas109::Parity CSerialPortParity = itas109::Parity::ParityNone;
	if (parity == "Even") {
		CSerialPortParity = itas109::Parity::ParityEven;
	}else if (parity == "None") {
		CSerialPortParity = itas109::Parity::ParityNone;
	}else if (parity == "Odd") {
		CSerialPortParity = itas109::Parity::ParityOdd;
	}
	
	// Create connection
	std::shared_ptr< itas109::CSerialPort> connect = std::make_shared<itas109::CSerialPort>();
	connect->init(port, baudrate, CSerialPortParity, CSerialPortDataBits, CSerialPortStopBits, CSerialPortFlowControl);

	// Open and save
	if (connect->open()) {
		connections[port] = connect;
		return USB_STATUS_CONNECTED;
	}else {
		return USB_STATUS_DISCONNECTED;
	}
}

std::vector<std::string> Tools_Hardware_USB_getAllPorts() {
	std::vector<std::string> ports;
	auto infos = itas109::CSerialPortInfo::availablePortInfos();
	for (const auto& info : infos) {
		ports.push_back(info.portName);
	}
	return ports;
}

std::string Tools_Hardware_USB_getConnectedPorts() {
	std::string ports = "";
	size_t maxConnections = connections.size();
	size_t count = 0;
	for (const auto& connection : connections) {
		ports += connection.first;
		if (count < maxConnections - 1) {
			ports += '\0';
		}
		count++;
	}
	return ports;
}

std::string Tools_Hardware_USB_getProtocolFromPort(const char port[]) {
	size_t index = Tools_Hardware_USB_getPortIndex(port);
	return PROTOCOL_STRING[Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[index].protocolIndex];
}

int32_t Tools_Hardware_USB_write(const char port[], const uint8_t data[], const uint16_t size, const int32_t timeout_ms) {
    if(!Tools_Hardware_USB_checkIfExist(port)){
      return -1;
    }
	const int32_t transmittedBytes = connections[port]->writeData(data, size);
	return transmittedBytes;
}

int32_t Tools_Hardware_USB_read(const char port[], uint8_t data[], const uint16_t size, const int32_t timeout_ms) {
    if(!Tools_Hardware_USB_checkIfExist(port)){
      return -1;
    }
	int32_t countedBytes = 0;
	int32_t leftBytes = size;
	long long endTime = Tools_Software_Algorithms_getMilliSeconds() + timeout_ms;
	while (countedBytes < size && endTime > Tools_Software_Algorithms_getMilliSeconds()) {
		const int32_t received = connections[port]->readData(data + countedBytes, leftBytes);
		countedBytes += received;
		leftBytes -= received;
	}
	return countedBytes;
}

void Tools_Hardware_USB_flush(const char port[]) {
	connections[port]->flushBuffers();
	connections[port]->flushReadBuffers();
	connections[port]->flushWriteBuffers();
}