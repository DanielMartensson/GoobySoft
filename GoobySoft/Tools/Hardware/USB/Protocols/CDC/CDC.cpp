#include "CDC.h"
#include <boost/asio.hpp>
#include <map>
#include <memory>
#include <chrono>
#include <cstdio>
#include "../../../../Tools.h"

static boost::asio::io_context io;
static std::map<std::string, std::shared_ptr<boost::asio::serial_port>> devicesCDC;

static bool CDCDeviceExist(const char port[]) {
	return devicesCDC.find(port) != devicesCDC.end() ? true : false;
}

static void checkPort(std::vector<std::string>& ports, const char port[]) {
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

static void tryPort(std::vector<std::string>& ports, const char portTemplate[]) {
	char port[MAX_C_STRING_LEN];
	for (int i = 0; i < 127; i++) {
		try {
			std::sprintf(port, "%s%i", portTemplate, i);
#if(defined(_GOOBYSOFT_DEBUG))
			std::printf("Reading port %s\n", port);
#endif
			checkPort(ports, port);
#if(defined(_GOOBYSOFT_DEBUG))
			std::printf("Port %s is available, collecting it\n", port);
#endif
		}
		catch (const boost::system::system_error& ex) {
			if (boost::asio::error::no_permission == ex.code().value() || boost::asio::error::access_denied == ex.code().value()) {
#if(defined(_GOOBYSOFT_DEBUG))
				std::printf("Port %s is bussy, collecting it\n", port);
#endif
				ports.push_back(port); // The port is bussy, it can be used.
			}
#if(defined(_GOOBYSOFT_DEBUG))
			else {
				std::printf("Port %s is not available. Error code: %i\n", port, ex.code().value());
			}
#endif
		}
	}
}

std::vector<std::string> Tools_Hardware_USB_Protocols_CDC_getAllPorts() {
	// Get the ports
	std::vector<std::string> ports;
	tryPort(ports, "COM");
	tryPort(ports, "/dev/ttyACM");
	tryPort(ports, "/dev/ttyUSB");
#if(defined(_GOOBYSOFT_DEBUG))
	std::printf("Total ports found: %i\n", (int)ports.size());
	for (size_t i = 0; i < ports.size(); i++) {
		std::printf("Port: %s\n", ports.at(i).c_str());
	}
#endif
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
	std::shared_ptr<boost::asio::serial_port> addDevice = std::make_shared<boost::asio::serial_port>(io, port);

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

std::string Tools_Hardware_USB_Protocols_CDC_getPortsOfConnectedDevices() {
	std::string ports;
	size_t maxDevicesCDC = devicesCDC.size();
	size_t count = 0;
	for (auto const& [port, devices] : devicesCDC) {
		ports += port.c_str();
		if (count < maxDevicesCDC - 1) {
			ports += '\0';
		}
		count++;
	}
	return ports;
}

#include <iostream>

std::vector<uint8_t> Tools_Hardware_USB_Protocols_CDC_startTransieveProcesss(const char port[], const long long timeOutMilliseconds, uint8_t dataTX[], size_t size) {
	std::vector<uint8_t> dataRX;
	if (CDCDeviceExist(port)) {
		// Check if data has been received or timeout has occurred
		auto startTime = std::chrono::steady_clock::now();
		auto timeoutDuration = std::chrono::milliseconds(timeOutMilliseconds);

		// Stop variable for the loop
		bool dataReceived = false;

		// Buffer for the read
		std::array<char, 1024> buffer;

		// Write data
		auto usbPort = devicesCDC.at(port);
		usbPort->write_some(boost::asio::buffer(dataTX, size));

		// Read the data
		while (!dataReceived) {
			usbPort->async_read_some(boost::asio::buffer(buffer), [&](const boost::system::error_code& error, std::size_t bytes_transferred) {
				if (!error) {
					dataRX.assign(buffer.begin(), buffer.begin() + bytes_transferred);
					dataReceived = true;
				}
			});

			// Check if timeout has occurred
			auto currentTime = std::chrono::steady_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);
			if (elapsedTime >= timeoutDuration) {
				dataReceived = true; // Kill the loop
			}
		}
	}
	return dataRX;
}