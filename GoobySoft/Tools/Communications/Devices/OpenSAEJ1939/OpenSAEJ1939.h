#ifndef OpenSAEJ1939
#define OpenSAEJ1939
#include <string>
#include <vector>

#define AUXILIARY_VALVE_COMMAND 16
#define AUXILIARY_VALVE_ESTIMATED_FLOW 16
#define AUXILIARY_VALVE_MEASURED_POSITION 16

// Constant fields
extern inline const std::vector<std::string> HEADERS_OpenSAEJ1939 = { "Port", "CAN address", "Function", "Display name" };
extern inline const std::vector<std::string> CAN_ADDRESSES_OpenSAEJ1939 = [] {
	std::vector<std::string> canAddress;
	for (int i = 0; i <= 255; i++) { canAddress.emplace_back(std::to_string(i)); }
	return canAddress;
}();
extern inline const std::vector<std::string> FUNCTION_NAMES_OpenSAEJ1939 = [] {
	std::vector<std::string> functionNames;
	for (int i = 0; i < AUXILIARY_VALVE_COMMAND; i++) { functionNames.emplace_back("Write auxiliary valve command " + std::to_string(i)); }
	for (int i = 0; i < AUXILIARY_VALVE_ESTIMATED_FLOW; i++) { functionNames.emplace_back("Read auxiliary valve estimated flow " + std::to_string(i)); }
	for (int i = 0; i < AUXILIARY_VALVE_MEASURED_POSITION; i++) { functionNames.emplace_back("Read auxiliary valve measured position " + std::to_string(i)); }
	// Add more...
	return functionNames;
}();

std::vector<std::vector<std::string>>& Tools_Communications_Devices_OpenSAEJ1939_getConfigurationTable();
std::string Tools_Communications_Devices_OpenSAEJ1939_getPort(size_t i);
std::string Tools_Communications_Devices_OpenSAEJ1939_getCANAddress(size_t i);
std::string Tools_Communications_Devices_OpenSAEJ1939_getFunction(size_t i);
std::string Tools_Communications_Devices_OpenSAEJ1939_getDisplayName(size_t i);
std::vector<std::string> Tools_Communications_Devices_OpenSAEJ1939_getDisplayNames();
size_t Tools_Communications_Devices_OpenSAEJ1939_getConfigurationTableDataSize();
bool Tools_Communications_Devices_OpenSAEJ1939_isInput(size_t i);
float* Tools_Communications_Devices_OpenSAEJ1939_getControlVariable(size_t i, float* minValue, float* maxValue);
float Tools_Communications_Devices_OpenSAEJ1939_setControlVariable(float value, size_t i);
float Tools_Communications_Devices_OpenSAEJ1939_getMeasurementVariable(size_t i);

#endif // !OpenSAEJ1939
