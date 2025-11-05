#ifndef Devices
#define Devices
#include "../../Storage/Parameters/Parameters.h"
#include <string>

// What protocol are the devices talking. Don't forget to update void Tools_Communications_Devices_updatePorts() as well!
static const std::string PROTOCOL_STRING[] = { "Modbus RTU", "Raw USB", "Modbus TCP"};
typedef enum {
	PROTOCOL_ENUM_MODBUS_RTU,
	PROTOCOL_ENUM_RAW_USB,
	PROTOCOL_ENUM_MODBUS_TCP,
}PROTOCOL_ENUM;

// Devices
#include "ADL400/ADL400.h"
#include "STM32PLC/STM32PLC.h"
#include "Uponor-X-148/Uponor-X-148.h"
#include "WellerToJCB/WellerToJBC.h"

TableColumnID Tools_Communications_Devices_createTableIDs(const char columName[], COLUMN_DEFINITION columnDefinition);
void Tools_Communications_Devices_createDevices();
void Tools_Communications_Devices_updatePorts();
void Tools_Communications_Devices_clear();

#endif // !Devices
