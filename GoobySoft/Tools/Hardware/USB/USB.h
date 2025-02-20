#ifndef USB
#define USB
#include <string>
#include <vector>

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

static const std::string USB_PARITY_STRING[] = { "None", "Odd", "Even" };
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

static const std::string USB_STOP_BITS_STRING[] = { "One", "Two" };
typedef enum {
	USB_STOP_BITS_ENUM_ONE,
	USB_STOP_BITS_ENUM_TWO
}USB_STOP_BITS_ENUM;

bool Tools_Hardware_USB_checkIfExist(const char port[]);
size_t Tools_Hardware_USB_getPortIndex(const char port[]);
USB_STATUS Tools_Hardware_USB_isConnected(const char port[]);
USB_STATUS Tools_Hardware_USB_closeConnection(const char port[]);
USB_STATUS Tools_Hardware_USB_openConnection(const char port[], const unsigned int baudrate, const unsigned int dataBits, const std::string& flowControl, const std::string& stopBits, const std::string& parity, const std::string& protocols);
std::vector<std::string> Tools_Hardware_USB_getAllPorts();
std::string Tools_Hardware_USB_getConnectedPorts();
std::string Tools_Hardware_USB_getProtocolFromPort(const char port[]);
int32_t Tools_Hardware_USB_write(const char port[], const uint8_t data[], const uint16_t size, const int32_t timeout_ms);
int32_t Tools_Hardware_USB_read(const char port[], uint8_t data[], const uint16_t size, const int32_t timeout_ms);
#endif // !USB