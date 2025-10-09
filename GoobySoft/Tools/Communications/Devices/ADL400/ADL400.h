#ifndef ADL400
#define ADL400
#include "../../../Tools.h"

std::vector<TableColumnID> Tools_Communications_Devices_ADL400_getTableColumnIDs();
COLUMN_FUNCTION Tools_Communications_Devices_ADL400_getColumnFunction(int functionValueIndex);
std::string Tools_Communications_Devices_ADL400_getFunctionValues();
float Tools_Communications_Devices_ADL400_getInput(const char port[], int functionValueIndex, int address);
bool Tools_Communications_Devices_ADL400_setOutput(const char port[], int functionValueIndex, int address, int value);

#endif // !ADL400
