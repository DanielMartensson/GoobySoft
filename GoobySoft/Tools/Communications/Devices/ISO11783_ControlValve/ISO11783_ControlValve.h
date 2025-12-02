#ifndef ISO11783_ControlValve
#define ISO11783_ControlValve
#include "../../../Tools.h"

std::string Tools_Communications_Devices_ISO11783_ControlValve_getFunctionValues();
std::vector<TableColumnID> Tools_Communications_Devices_ISO11783_ControlValve_getTableColumnIDs();
float Tools_Communications_Devices_ISO11783_ControlValve_getInput(const char port[], int functionValueIndex, int address);
bool Tools_Communications_Devices_ISO11783_ControlValve_setOutput(const char port[], int functionValueIndex, int address, int value);
COLUMN_FUNCTION Tools_Communications_Devices_ISO11783_ControlValve_getColumnFunction(int functionValueIndex);

#endif // !ISO11783_ControlValve
