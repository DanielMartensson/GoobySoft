#ifndef STM32PLC
#define STM32PLC
#include "../../../Tools.h"

std::vector<TableColumnID> Tools_Communications_Devices_STM32PLC_getTableColumnIDs();
COLUMN_FUNCTION Tools_Communications_Devices_STM32PLC_getColumnFunction(int functionValueIndex);
std::string Tools_Communications_Devices_STM32PLC_getFunctionValues();
float Tools_Communications_Devices_STM32PLC_getInput(const char port[], int functionValueIndex, int address);
bool Tools_Communications_Devices_STM32PLC_setOutput(const char port[], int functionValueIndex, int address, int value);

#endif // !STM32PLC
