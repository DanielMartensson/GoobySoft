#include "USB.h"
#include "../../Tools.h"
#include <CSerialPort/SerialPort.h>
#include <CSerialPort/SerialPortInfo.h>

// Map for all connections of ports and collections of data
static std::map<std::string, std::shared_ptr<itas109::CSerialPort>> portConnections;
static std::map<std::string, std::vector<uint8_t>> dataCollections;
static std::map<std::string, std::vector<uint32_t>> packageSizes;

// This is the total length and size of both dataCollections and packageSizes
#define MAX_PACKAGE_LENGTH_SIZE 1024

class USB_Listener : public itas109::CSerialPortListener {
	private:
    itas109::CSerialPort *p_sp;

	public:
    USB_Listener(itas109::CSerialPort *sp) : p_sp(sp){

	};

    void onReadEvent(const char *port, unsigned int dataBuffer) {
        if (dataBuffer == 0){
			return;
		}

		// Store data
		std::vector<uint8_t>& dataCollection = dataCollections[port];
		
		// Read the total data to \0 or let the timeout occur
		uint8_t data[MAX_PACKAGE_LENGTH_SIZE];
		uint32_t totalReceivedPackage = 0;
		while(true){
			int received = p_sp->readData(data, dataBuffer);
			if(received > 0){
				totalReceivedPackage += received;
				dataCollection.insert(dataCollection.end(), data, data + received);
				if(data[received - 1] == '\0'){
					break; // Null termination reached! Raw USB operates here
				}
			}else if(received == 0){
				break; // Timeout! MODBUS, CAN etc. operates here
			}else{
				break; // An error occurs
			}
		}

		// Remember package size
		std::vector<uint32_t>& packageSize = packageSizes[port];
		if(packageSize.size() >= MAX_PACKAGE_LENGTH_SIZE){
			// Overflow: Delete the first package and first data bytes in the data collection
			const uint32_t sizeOfPackage = packageSize.front();
			dataCollection.erase(dataCollection.begin(), dataCollection.begin() + sizeOfPackage);
			packageSize.erase(packageSize.begin());
		}
		packageSize.push_back(totalReceivedPackage);
    };
};

static std::map<std::string, std::shared_ptr<USB_Listener>> portListeners;

bool Tools_Hardware_USB_checkIfExist(const char port[]) {
	return portConnections.find(port) != portConnections.end() ? true : false;
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
		return portConnections[port]->isOpen() ? USB_STATUS_CONNECTED : USB_STATUS_NOT_CONNECTED;
	}else {
		return USB_STATUS_NOT_EXIST;
	}
}

USB_STATUS Tools_Hardware_USB_closeConnection(const char port[]) {
	if (Tools_Hardware_USB_checkIfExist(port)) {
		if (Tools_Hardware_USB_isConnected(port)) {
			portConnections[port]->close();
		}
		portConnections.erase(port);
		portListeners.erase(port);
		dataCollections.erase(port);
		packageSizes.erase(port);
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
	
	// Create port connection
	std::shared_ptr<itas109::CSerialPort> portConnection = std::make_shared<itas109::CSerialPort>();
	portConnection->init(port, baudrate, CSerialPortParity, CSerialPortDataBits, CSerialPortStopBits, CSerialPortFlowControl);

	// Open
	if (portConnection->open()) {
		// Save
		portConnections[port] = portConnection;
		dataCollections[port] = {};
		packageSizes[port] = {};

		// Start a listener for the port
		std::shared_ptr<USB_Listener> portListener = std::make_shared<USB_Listener>(portConnection.get());
		portConnection->connectReadEvent(portListener.get());
		portListeners[port] = portListener;

		// OK
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
	size_t maxPortConnections = portConnections.size();
	size_t count = 0;
	for (const auto& connection : portConnections) {
		ports += connection.first;
		if (count < maxPortConnections - 1) {
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
	const int32_t transmittedBytes = portConnections[port]->writeData(data, size);
	return transmittedBytes;
}

void Tools_Hardware_USB_setTimeout(const char port[], const uint32_t timeout_ms){
	if(!Tools_Hardware_USB_checkIfExist(port)){
		return;
    }
	portConnections[port]->setReadIntervalTimeout(timeout_ms);
}

uint32_t Tools_Hardware_USB_getTimeout(const char port[]){
	if(!Tools_Hardware_USB_checkIfExist(port)){
      return 0;
    }
	return portConnections[port]->getReadIntervalTimeout();
}

int32_t Tools_Hardware_USB_read_bytes(const char port[], uint8_t data[], const uint16_t size, const int32_t timeout_ms) {
    if(!Tools_Hardware_USB_checkIfExist(port)){
      return -1;
    }
	if(Tools_Hardware_USB_getTimeout(port) != timeout_ms){
		Tools_Hardware_USB_setTimeout(port, timeout_ms);
	}
	const int32_t countedBytes = portConnections[port]->readData(data, size);
	return countedBytes;

	/*
	int32_t countedBytes = 0;
	int32_t leftBytes = size;
	long long endTime = Tools_Software_Algorithms_getMilliSeconds() + timeout_ms;
	while (countedBytes < size && endTime > Tools_Software_Algorithms_getMilliSeconds()) {
		const int32_t received = portConnections[port]->readData(data + countedBytes, leftBytes);
		countedBytes += received;
		leftBytes -= received;
	}
	return countedBytes;*/
}

int32_t Tools_Hardware_USB_read_packages(const char port[], uint8_t data[]) {
    if(!Tools_Hardware_USB_checkIfExist(port)){
      return -1;
    }
	
	// Check available package size
	std::vector<uint32_t>& packageSize = packageSizes[port];
	if(packageSize.empty()){
		return 0;
	}
	const uint32_t sizeOfPackage = packageSize.front();

	// Copy over
	std::vector<uint8_t>& dataCollection = dataCollections[port];
	std::copy(dataCollection.begin(), dataCollection.begin() + sizeOfPackage, data);

	// Delete
	dataCollection.erase(dataCollection.begin(), dataCollection.begin() + sizeOfPackage);
	packageSize.erase(packageSize.begin());

	// Allways return the size of the package, even if it's empty
	return sizeOfPackage;
}

void Tools_Hardware_USB_flush(const char port[]) {
    if(!Tools_Hardware_USB_checkIfExist(port)){
      return;
    }
	portConnections[port]->flushBuffers();
	portConnections[port]->flushReadBuffers();
	portConnections[port]->flushWriteBuffers();
	dataCollections[port].clear();
	packageSizes[port].clear();
}