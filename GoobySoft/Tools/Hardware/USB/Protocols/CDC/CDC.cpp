#include "CDC.h"
#include <boost/asio.hpp>
#include <map>
#include <cstdio>
#include "../../USB.h"

boost::asio::io_service io;
std::map<std::string, std::shared_ptr<boost::asio::serial_port>> devicesCDC;

bool CDCDeviceExist(const char port[]) {
	return devicesCDC.find(port) != devicesCDC.end() ? true : false;
}

void checkPort(std::vector<std::string>& ports, char port[]) {
	// Check if it already exist 
	if (CDCDeviceExist(port)) {
		if (devicesCDC.at(port)->is_open()) {
			ports.push_back(port);
		}
	}
	else {
		// Create temporary port
		boost::asio::serial_port addDevice(io);
		addDevice.open(port);
		if (addDevice.is_open()) {
			ports.push_back(port);
		}
		addDevice.close();
	}
}

std::vector<std::string> Tools_Hardware_USB_Protocols_CDC_getAllPorts() {
	// Get the names
	char port[20];
	std::vector<std::string> ports;
	for (int i = 0; i < 127; i++) {
		try {
			std::sprintf(port, "COM%i", i);
			checkPort(ports, port);
		}
		catch (const boost::system::system_error& ex) {
			if (boost::asio::error::no_permission == ex.code().value()) {
				ports.push_back(port); // If we scanned the port and the port is bussy, that means it can be used
			}
		}
	}
	for (int i = 0; i < 127; i++) {
		try {
			std::sprintf(port, "/dev/ttyACM%i", i);
			checkPort(ports, port);
		}
		catch (const boost::system::system_error& ex) {
			if (boost::asio::error::no_permission == ex.code().value()) {
				ports.push_back(port); // If we scanned the port and the port is bussy, that means it can be used
			}
		}
	}
	for (int i = 0; i < 127; i++) {
		try {
			std::sprintf(port, "/dev/ttyUSB%i", i);
			checkPort(ports, port);
		}
		catch (const boost::system::system_error& ex) {
			if (boost::asio::error::no_permission == ex.code().value()) {
				ports.push_back(port); // If we scanned the port and the port is bussy, that means it can be used
			}
		}
	}
	return ports;
}

bool Tools_Hardware_USB_Protocols_CDC_closeConnection(const char port[]) {
	if (Tools_Hardware_USB_Protocols_CDC_isConnected(port)) {
		devicesCDC.at(port)->close();
		devicesCDC.erase(port);
		return true;
	}
	else {
		if (CDCDeviceExist(port)) {
			devicesCDC.erase(port);
		}
		return false;
	}
}

bool Tools_Hardware_USB_Protocols_CDC_openConnection(const char port[], const unsigned int baudrate, const unsigned int dataBits, const unsigned int flowControl, const unsigned int stopBits, const unsigned int parity) {
	// Open connection
	std::shared_ptr<boost::asio::serial_port> addDevice(new boost::asio::serial_port(io, port));

	// Baudrate options
	addDevice->set_option(boost::asio::serial_port::baud_rate(boost::asio::serial_port_base::baud_rate(baudrate)));

	// Data bits options
	addDevice->set_option(boost::asio::serial_port::character_size(boost::asio::serial_port_base::character_size(dataBits)));

	// Control flow options
	boost::asio::serial_port::flow_control controlFlowNone(boost::asio::serial_port_base::flow_control::none);
	boost::asio::serial_port::flow_control controlFlowSoftware(boost::asio::serial_port_base::flow_control::software);
	boost::asio::serial_port::flow_control controlFlowHardware(boost::asio::serial_port_base::flow_control::hardware);
	switch (flowControl) {
	case USB_CONTROL_FLOW_ENUM_NONE:
		addDevice->set_option(controlFlowNone);
		break;
	case USB_CONTROL_FLOW_ENUM_SOFTWARE:
		addDevice->set_option(controlFlowSoftware);
		break;
	case USB_CONTROL_FLOW_ENUM_HARDWARE:
		addDevice->set_option(controlFlowHardware);
		break;
	default:
		addDevice->set_option(controlFlowNone);
		break;
	}

	// Stop bits options
	boost::asio::serial_port_base::stop_bits stop_bits_one(boost::asio::serial_port_base::stop_bits::one);
	boost::asio::serial_port_base::stop_bits stop_bits_onepointfive(boost::asio::serial_port_base::stop_bits::onepointfive);
	boost::asio::serial_port_base::stop_bits stop_bits_two(boost::asio::serial_port_base::stop_bits::two);
	switch (stopBits) {
	case USB_STOP_BITS_ENUM_ONE:
		addDevice->set_option(stop_bits_one);
		break;
	case USB_STOP_BITS_ENUM_TWO:
		addDevice->set_option(stop_bits_two);
		break;
	default:
		addDevice->set_option(stop_bits_one);
		break;
	}

	// Parity options
	boost::asio::serial_port_base::parity parity_none(boost::asio::serial_port_base::parity::none);
	boost::asio::serial_port_base::parity parity_odd(boost::asio::serial_port_base::parity::odd);
	boost::asio::serial_port_base::parity parity_even(boost::asio::serial_port_base::parity::even);
	switch (parity) {
	case USB_PARITY_ENUM_NONE:
		addDevice->set_option(parity_none);
		break;
	case USB_PARITY_ENUM_ODD:
		addDevice->set_option(parity_odd);
		break;
	case USB_PARITY_ENUM_EVEN:
		addDevice->set_option(parity_even);
		break;
	default:
		addDevice->set_option(parity_none);
		break;
	}

	// Store the addDevice
	devicesCDC[port] = addDevice;
	return Tools_Hardware_USB_Protocols_CDC_isConnected(port);
}

bool Tools_Hardware_USB_Protocols_CDC_isConnected(const char port[]) {
	bool isOpen = false;
	if (CDCDeviceExist(port)) {
		isOpen = devicesCDC.at(port)->is_open();
	}
	return isOpen;
}

std::vector<std::string> Tools_Hardware_USB_Protocols_CDC_getPortsOfConnectedDevices() {
	std::vector<std::string> ports;
	for (auto const& [port, addDevice] : devicesCDC) {
		ports.push_back(port);
	}
	return ports;
}

void Tools_Hardware_USB_Protocols_CDC_writeData(const char port[], const uint8_t data[], const size_t length) {
	if (CDCDeviceExist(port)) {
		auto writeHandler = [&](const boost::system::error_code& errorCode, std::size_t bytesTransferred) {};
		devicesCDC.at(port)->async_write_some(boost::asio::buffer(data, length), writeHandler);
	}
}

std::vector<uint8_t> Tools_Hardware_USB_Protocols_CDC_readData(const char port[], const long long timeOutMicroseconds) {
	std::vector<uint8_t> data;
	if (CDCDeviceExist(port)) {

		// Create clock
		boost::asio::steady_timer timer(io, std::chrono::milliseconds(timeOutMicroseconds));
		
		// Create read handler
		auto readHandler = [&](const boost::system::error_code& errorCode, std::size_t bytesTransferred) {};

		// Read
		devicesCDC.at(port)->async_read_some(boost::asio::buffer(data), readHandler);
		
		// Start asyncronus timer
		timer.async_wait([&](const boost::system::error_code& errorCode) {});

		// Run
		io.run();

	}
	return data;
}
