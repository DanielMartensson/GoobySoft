#ifndef USBProtocols
#define USBProtocols

#include "ModbusRTU/ModbusRTU.h"
#include "CDC/CDC.h"

static const std::string USB_PROTOCOL_STRING[] = { "Modbus RTU", "CDC"};
typedef enum {
	USB_PROTOCOL_ENUM_MODBUS_RTU,
	USB_PROTOCOL_ENUM_CDC
}USB_PROTOCOL_ENUM;

#endif // !USBProtocols
