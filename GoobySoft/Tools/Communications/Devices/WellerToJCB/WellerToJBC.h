#ifndef WellerToJCB
#define WellerToJCB
#include "../../Tools/Tools.h"

std::string Tools_Communications_Devices_WellerToJBC_getFunctionValues();
std::vector<TableColumnID> Tools_Communications_Devices_WellerToJBC_getTableColumnIDs();
float Tools_Communications_Devices_WellerToJBC_getInput(const char port[], int functionValueIndex, int address);
bool Tools_Communications_Devices_WellerToJBC_setOutput(const char port[], int functionValueIndex, int address, int value);
COLUMN_FUNCTION Tools_Communications_Devices_WellerToJBC_getColumnFunction(int functionValueIndex);

#endif // !WellerToJCB
