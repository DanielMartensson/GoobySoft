#ifndef UPONOR_X_148_H
#define UPONOR_X_148_H
#include "../../../Tools.h"

std::vector<TableColumnID> Tools_Communications_Devices_Uponor_X_148_getTableColumnIDs();
COLUMN_FUNCTION Tools_Communications_Devices_Uponor_X_148_getColumnFunction(int functionValueIndex);
std::string Tools_Communications_Devices_Uponor_X_148_getFunctionValues();
float Tools_Communications_Devices_Uponor_X_148_getInput(const char port[], int functionValueIndex, int address);
bool Tools_Communications_Devices_Uponor_X_148_setOutput(const char port[], int functionValueIndex, int address, int value);


#endif // !UPONOR_X_148_H