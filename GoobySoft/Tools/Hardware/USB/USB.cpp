#include "USB.h"
#include "../../Tools.h"
#include <CSerialPort/SerialPort.h>
#include <CSerialPort/SerialPortInfo.h>

// Map of port connections
static std::map<std::string, std::shared_ptr<itas109::CSerialPort>> portConnections;

// Maps of the circular buffert
#define MAX_DATA_COLLECTION 1024
static std::map<std::string, std::vector<uint8_t>> dataCollections;

// Inline function for directly read. This is both used by the listener and the Tools_Hardware_USB_read function 
static inline int readDataDirectly(const char port[], unsigned int bytesAvailable){
    // Don't read 0 bytes
	if (bytesAvailable == 0){
		return 0;
	}

	// Get port 
	itas109::CSerialPort* p_sp = portConnections[port].get();

	// Read the data
	uint8_t bytesBuffer[MAX_DATA_COLLECTION];
	const int bytesRead = MAX_DATA_COLLECTION < bytesAvailable ? MAX_DATA_COLLECTION : bytesAvailable;
	const int receivedBytes = p_sp->readData(bytesBuffer, bytesRead);
	if(receivedBytes <= 0){
		return 0; 
	}

	// Store the data from buffer in a vector
	std::vector<uint8_t>& dataCollection = dataCollections[port];
	dataCollection.insert(dataCollection.end(),bytesBuffer, bytesBuffer + receivedBytes);

	// If the collection is to large, remove the first part
	int32_t dataDollectionSize = Tools_Hardware_USB_availableBytes(port);
	if(dataDollectionSize > MAX_DATA_COLLECTION){
    	const size_t difference = dataDollectionSize - MAX_DATA_COLLECTION;
		dataCollection.erase(dataCollection.begin(), dataCollection.begin() + difference);
	}

	// Return recieved bytes
	return receivedBytes;
}

class USB_Listener : public itas109::CSerialPortListener {
	private:
    itas109::CSerialPort *p_sp;

	public:
    USB_Listener(itas109::CSerialPort *sp) : p_sp(sp){};

    void onReadEvent(const char port[], unsigned int bytesAvailable) {
		readDataDirectly(port, bytesAvailable);
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

		// Start a listener for the port
		std::shared_ptr<USB_Listener> portListener = std::make_shared<USB_Listener>(portConnection.get());
		portConnection->connectReadEvent(portListener.get());
		portListeners[port] = portListener;
		portConnections[port]->setReadIntervalTimeout(10);

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

int32_t Tools_Hardware_USB_write(const char port[], const uint8_t data[], const uint16_t elements, const int32_t timeout_ms) {
    if(!Tools_Hardware_USB_checkIfExist(port)){
      return -1;
    }
	if(Tools_Hardware_USB_getTimeout(port) != timeout_ms){
		Tools_Hardware_USB_setTimeout(port, timeout_ms);
	}
	const int32_t transmittedBytes = portConnections[port]->writeData(data, elements);
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

int32_t Tools_Hardware_USB_read(const char port[], uint8_t data[], const uint16_t elements, const int32_t timeout_ms, const bool eraseDataAfterRead, const bool readDataLatestAvailable) {
    if(!Tools_Hardware_USB_checkIfExist(port)){
      return -1;
    }

	// Check available bytes
	int32_t availableBytes = Tools_Hardware_USB_availableBytes(port);

	// When timeout is equal to 0 and elements is larger than available bytes
	if(timeout_ms == 0 && (availableBytes < elements)){
		Tools_Hardware_USB_flush(port);
		return 0;
	}

	// Read manually to see if we need to run that while-statement or not
	readDataDirectly(port, elements); 
	availableBytes = Tools_Hardware_USB_availableBytes(port);

	// Timeout is larger than 0, that means we need to read data
	uint16_t time_ms = 0;
	while(availableBytes < elements){
		// Sleep
		Tools_Software_Algorithms_goobySleep_ms(1);

		/* 
		 * Read the data before the listerner does.
		 * The listener works fine for heartbeat messages such as CAN-bus.
		 * But not for MODBUS, the reading must be manually.
		 */
		readDataDirectly(port, elements); 
		availableBytes = Tools_Hardware_USB_availableBytes(port);

		// Cound the time out
		time_ms++;
		if(time_ms > timeout_ms){
			return 0;
		}
	}

	// Copy data from the last element
	std::vector<uint8_t>& dataCollection = dataCollections[port];
	if(readDataLatestAvailable){
		std::copy(dataCollection.end() - elements, dataCollection.end(), data);
	}else{
		std::copy(dataCollection.begin(), dataCollection.begin() + elements, data);
	}

	// If erase data is true
	if(eraseDataAfterRead){
		Tools_Hardware_USB_eraseData(port, 0, elements);
	}

	// Return the size. It's up to the Devices to determine(use protocol) if this uint8_t data[] is complete or not
	return elements;
}

bool Tools_Hardware_USB_eraseData(const char port[], const uint16_t startIndex, const uint16_t elements){
	if(!Tools_Hardware_USB_checkIfExist(port)){
		return false;
    }

	// Check data
	if(startIndex + elements > Tools_Hardware_USB_availableBytes(port)) {
		return false;
    }

	// Delete data
	std::vector<uint8_t>& dataCollection = dataCollections[port];
	dataCollection.erase(dataCollection.begin() + startIndex, dataCollection.begin() + startIndex + elements);

	// OK
	return true;
}

void Tools_Hardware_USB_flush(const char port[]) {
    if(!Tools_Hardware_USB_checkIfExist(port)){
      return;
    }
	portConnections[port]->flushBuffers();
	portConnections[port]->flushReadBuffers();
	portConnections[port]->flushWriteBuffers();
	dataCollections[port].clear();
}

int32_t Tools_Hardware_USB_availableBytes(const char port[]){
	if(!Tools_Hardware_USB_checkIfExist(port)){
		return -1;
    } 
	return (int32_t)dataCollections[port].size();
}