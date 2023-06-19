#ifndef USB
#define USB
#define MAX_USB_DEVICES 127
#include <string>
#include <vector>
#include "Protocols/Protocols.h"

typedef enum {
	USB_STATUS_FAIL,
	USB_STATUS_OK,
	USB_STATUS_CONNECTED,
	USB_STATUS_NOT_EXIST,
	USB_STATUS_DISCONNECTED,
	USB_STATUS_NOT_CONNECTED,
	USB_STATUS_MISSING_DATA,
	USB_STATUS_NO_PROTOCOL
}USB_STATUS;

static const std::string USB_PROTOCOL_STRING[] = { "Modbus", "CDC" };
typedef enum {
	USB_PROTOCOL_ENUM_MODBUS,
	USB_PROTOCOL_ENUM_CDC
}USB_PROTOCOL_ENUM;

static const std::string USB_PARITY_STRING[] = { "None", "Odd", "Even"};
typedef enum {
	USB_PARITY_ENUM_NONE,
	USB_PARITY_ENUM_ODD,
	USB_PARITY_ENUM_EVEN,
}USB_PARITY_ENUM;

static const std::string USB_CONTROL_FLOW_STRING[] = { "None", "Software", "Hardware" };
typedef enum {
	USB_CONTROL_FLOW_ENUM_NONE,
	USB_CONTROL_FLOW_ENUM_SOFTWARE,
	USB_CONTROL_FLOW_ENUM_HARDWARE
}USB_CONTROL_FLOW_ENUM;

static const std::string USB_STOP_BITS_STRING[] = { "One", "Two"};
typedef enum {
	USB_STOP_BITS_ENUM_ONE,
	USB_STOP_BITS_ENUM_TWO
}USB_STOP_BITS_ENUM;

size_t Tools_Hardware_USB_getPortIndex(const char port[]);
USB_STATUS Tools_Hardware_USB_isConnected(const char port[], const std::string& protocol);
USB_STATUS Tools_Hardware_USB_closeConnection(const char port[], const std::string& protocol);
USB_STATUS Tools_Hardware_USB_openConnection(const char port[], const unsigned int baudrate, const unsigned int dataBits, const std::string& flowControl, const std::string& stopBits, const std::string& parity, const std::string& protocol);
std::vector<std::string> Tools_Hardware_USB_getAllPorts();
std::vector<std::string> Tools_Hardware_USB_getConnectedPorts(const std::string& protocol);
std::string Tools_Hardware_USB_getProtocolFromPort(const char port[]);
#endif