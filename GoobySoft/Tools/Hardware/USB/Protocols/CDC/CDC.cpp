#include "CDC.h"
#include <boost/asio.hpp>
#include <map>
#include <memory>
#include <chrono>
#include <cstdio>
#include "../../../../Tools.h"

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
		static boost::asio::io_context io;
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
	static boost::asio::io_context io;
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

int32_t Tools_Hardware_USB_Protocols_CDC_write(const char port[], const uint8_t data[], const uint16_t size, const int32_t timeout_ms) {
	int32_t writtenBytes = 0;
	if (CDCDeviceExist(port)) {

		// Get the USB
		auto deviceUSB = devicesCDC.at(port);

		// Utför skrivoperationen
		writtenBytes = boost::asio::write(*deviceUSB, boost::asio::buffer(data, size));
	}
	return writtenBytes;
}

int32_t Tools_Hardware_USB_Protocols_CDC_read(const char port[], uint8_t data[], const uint16_t size, const int32_t timeout_ms) {
	int32_t bytesRead = 0;
	if (CDCDeviceExist(port)) {

		// Get the USB
		auto deviceUSB = devicesCDC.at(port);

		// Important stuffs
		boost::asio::io_context io_context;
		boost::asio::deadline_timer timer(io_context);
		boost::system::error_code ec;

		// Create dead line timer
		timer.expires_from_now(boost::posix_time::milliseconds(timeout_ms));
		timer.async_wait([&](const boost::system::error_code& error) {
			if (!error) {
				// If time out occurs
				ec = boost::asio::error::operation_aborted;
				deviceUSB->cancel();
			}
			});

		// Start the thread
		std::thread io_thread([&]() { io_context.run(); });

		// Read with error code
		bytesRead = boost::asio::read(*deviceUSB, boost::asio::buffer(data, size), ec);

		// Cancle the timer and wait for the io_context to join
		timer.cancel();
		if (io_thread.joinable()) {
			io_thread.join();
		}
	}

	return bytesRead;
}

std::vector<uint8_t> Tools_Hardware_USB_Protocols_CDC_writeThenRead(const char port[], const long long timeoutMilliseconds, uint8_t dataTX[], size_t size, std::string endingOfDataRX) {
	std::vector<uint8_t> dataRX;
	if (CDCDeviceExist(port)) {

		// Get the USB
		auto deviceUSB = devicesCDC.at(port);

		// Flag
		bool wait;

		// Timer
		static boost::asio::io_context io;
		boost::asio::steady_timer timer(io, std::chrono::milliseconds(timeoutMilliseconds));
		timer.async_wait([&](boost::system::error_code ec) {
#ifndef _GOOBYSOFT_DEBUG
			std::cerr << "CDC.cpp - Timer completion (Code:" << ec.value() << " Message:" << ec.message() << ")" << std::endl;
#endif
			if (!ec) {
#ifndef _GOOBYSOFT_DEBUG
				std::cerr << "CDC.cpp - Timeout expired" << std::endl;
#endif
			}
			wait = false;
			});

		// Write 
		boost::asio::write(*deviceUSB, boost::asio::buffer(dataTX, size));

		// Read process
		boost::asio::async_read_until(*deviceUSB, boost::asio::dynamic_buffer(dataRX), endingOfDataRX, [&](boost::system::error_code ec, size_t bytes_transferred) {
#ifndef _GOOBYSOFT_DEBUG
			std::cerr << "CDC.cpp - Read completion (" << ec.message() << ", " << bytes_transferred << " bytes)" << std::endl;
#endif
			if (!ec.failed()) {
#ifndef _GOOBYSOFT_DEBUG
				std::cerr << "CDC.cpp: Reading data - OK" << std::endl;
				timer.cancel();
#endif
			}
			wait = false;
			});

		wait = true;
		io.run();

		// Wait
		while (wait) { ; }
		io.reset();
	}
	return dataRX;
}