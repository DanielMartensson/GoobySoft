#include "USB.h"
#include "../../Tools.h"
#include <CSerialPort/SerialPort.h>
#include <CSerialPort/SerialPortInfo.h>

// Map of port connections
static std::map<std::string, std::shared_ptr<itas109::CSerialPort>> portConnections;

// Maps of the circular buffert
#define MAX_DATA_COLLECTION 1024
static std::map<std::string, std::array<uint8_t, MAX_DATA_COLLECTION>> dataCollections;
static std::map<std::string, int> dataBeginIndexs;
static std::map<std::string, bool> dataIsNew;

class USB_Listener : public itas109::CSerialPortListener {
	private:
    itas109::CSerialPort *p_sp;

	public:
    USB_Listener(itas109::CSerialPort *sp) : p_sp(sp){

	};

    void onReadEvent(const char* port, unsigned int bytesAvailable) {
        if (bytesAvailable == 0){
			return;
		}
		
		// Read the data
		uint8_t bytesBuffer[MAX_DATA_COLLECTION];
		const int receivedBytes = p_sp->readData(bytesBuffer, bytesAvailable);
		if(receivedBytes < 0){
			return; // Error
		}

		// Store the data from buffer in a circular way
		Tools_Software_Algorithms_circularCopy(bytesBuffer, dataCollections[port].data(), 0, dataBeginIndexs[port], receivedBytes, receivedBytes, MAX_DATA_COLLECTION);

		// Shift index to new beginning index in a circular way
		dataBeginIndexs[port] = (dataBeginIndexs[port] + receivedBytes) % MAX_DATA_COLLECTION;

		// Set flag that the data is new
		dataIsNew[port] = true; 
    };
};

// Listener thread
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
		dataBeginIndexs.erase(port);
		dataIsNew.erase(port);
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
		dataBeginIndexs[port] = 0;
		dataIsNew[port] = false;

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

int32_t Tools_Hardware_USB_read(const char port[], uint8_t data[], const uint16_t size, const int32_t timeout_ms) {
    if(!Tools_Hardware_USB_checkIfExist(port)){
      return -1;
    }
	if(Tools_Hardware_USB_getTimeout(port) != timeout_ms){
		Tools_Hardware_USB_setTimeout(port, timeout_ms);
	}
	if(!dataIsNew[port]){
		return 0; // No data available
	}

	// Store the data in a circular way with size elements backwards as starting index
	Tools_Software_Algorithms_circularCopy(dataCollections[port].data(), data, dataBeginIndexs[port] - size, 0, size, MAX_DATA_COLLECTION, size);

	// Set flag that the data has been read
	dataIsNew[port] = false;

	// Return the size. It's up to the Devices to determine(use protocol) if this uint8_t data[] is complete or not
	return size;
}

void Tools_Hardware_USB_flush(const char port[]) {
    if(!Tools_Hardware_USB_checkIfExist(port)){
      return;
    }
	portConnections[port]->flushBuffers();
	portConnections[port]->flushReadBuffers();
	portConnections[port]->flushWriteBuffers();
	dataCollections[port].fill(0);
	dataBeginIndexs[port] = 0;
	dataIsNew[port] = false;
}