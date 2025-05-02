#ifndef WellerToJCB
#define WellerToJCB
#include "../../Tools/Tools.h"

std::string Tools_Communications_Devices_WellerToJBC_getFunctionValues();
std::vector<TableColumnID> Tools_Communications_Devices_WellerToJBC_getTableColumnIDs();
float Tools_Communications_Devices_WellerToJBC_getInput(const char port[], int functionValueIndex, int address);
bool Tools_Communications_Devices_WellerToJBC_setOutput(const char port[], int functionValueIndex, int address, int value);
COLUMN_FUNCTION Tools_Communications_Devices_WellerToJBC_getColumnFunction(int functionValueIndex);
bool Tools_Communications_Devices_WellerToJBC_setAnalogInputCalibration(const char port[], const int address, const int rawMin[], const int rawMax[], const float realMin[], const float realMax[]);
bool Tools_Communications_Devices_WellerToJBC_getAnalogInputCalibration(const char port[], const int address, int rawMin[], int rawMax[], float realMin[], float realMax[]);
bool Tools_Communications_Devices_WellerToJBC_setOperation(const char port[], const int address, const int operationIndex);
bool Tools_Communications_Devices_WellerToJBC_getOperation(const char port[], const int address, int* operationIndex);
bool Tools_Communications_Devices_WellerToJBC_getParameters(const char port[], const int address, float* A, float* B, float* qw, float* rv, float* qz, float* s, float* psi, float* alpha, float* antiwindup, float* umin, float* umax, float* steadyStateModelError, float* zmax, float* deltaumin, float* deltaumax);
bool Tools_Communications_Devices_WellerToJBC_setParameters(const char port[], const int address, const float A, const float B, const float qw, const float rv, const float qz, const float s, const float psi, const float alpha, const float antiwindup, const float umin, const float umax, const float steadyStateModelError, const float zmax, const float deltaumin, const float deltaumax);

#endif // !WellerToJCB
