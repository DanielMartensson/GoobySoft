#ifndef Devices
#define Devices
#include "../../Storage/Parameters/Parameters.h"
#include <string>

// What protocol are the devices talking
static const std::string PROTOCOL_STRING[] = { "Modbus RTU", "CDC", "Modbus TCP"};
typedef enum {
	USB_PROTOCOL_ENUM_MODBUS_RTU,
	USB_PROTOCOL_ENUM_CDC,
	NETWORK_PROTOCOL_ENUM_MODBUS_TCP
}PROTOCOL_ENUM;

// Devices
#include "ADL400/ADL400.h"
#include "STM32PLC/STM32PLC.h"
#include "Uponor-X-148/Uponor-X-148.h"

TableColumnID Tools_Communications_Devices_createTableIDs(const char columName[], COLUMN_DEFINITION columnDefinition);
void Tools_Communications_Devices_createDevices();
void Tools_Communications_Devices_updatePorts();
void Tools_Communications_Devices_clear();

#endif // !Devices
