#ifndef Devices
#define Devices
#include "../../Storage/Parameters/Parameters.h"

TableColumnID Tools_Communications_Devices_createTableIDs(const char columName[], COLUMN_DEFINITION columnDefinition);
void Tools_Communications_Devices_createDevices();
void Tools_Communications_Devices_updatePorts();
void Tools_Communications_Devices_clear();

#endif // !Devices
