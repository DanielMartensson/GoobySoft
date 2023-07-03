#ifndef ADL400
#define ADL400
#include <string>
#include <vector>

// Constant fields
extern inline const std::vector<std::string> HEADERS_ADL400 = { "Port", "Modbus address", "Function", "Display name" };
extern inline const std::vector<std::string> MODBUS_ADDRESSES_ADL400 = [] {
	std::vector<std::string> modbusAddress;
	for (int i = 1; i <= 254; i++) { modbusAddress.emplace_back(std::to_string(i)); }
	return modbusAddress;
}();
extern inline const std::vector<std::string> FUNCTION_NAMES_ADL400 = { "Read volt A", "Read volt B", "Read volt C", "Read current A", "Read current B", "Read current C", "Read frequency", "Read volt AB", "Read volt AC", "Read volt CB", "Total active power"};

std::vector<std::vector<std::string>>& Tools_Communications_Devices_ADL400_getConfigurationTable();
std::string Tools_Communications_Devices_ADL400_getPort(size_t i);
std::string Tools_Communications_Devices_ADL400_getSlaveAddress(size_t i);
std::string Tools_Communications_Devices_ADL400_getFunction(size_t i);
std::string Tools_Communications_Devices_ADL400_getDisplayName(size_t i);
std::vector<std::string> Tools_Communications_Devices_ADL400_getDisplayNames();
size_t Tools_Communications_Devices_ADL400_getConfigurationTableDataSize();
bool Tools_Communications_Devices_ADL400_isInput(size_t i);
int* Tools_Communications_Devices_ADL400_getControlVariable(size_t i);
float Tools_Communications_Devices_ADL400_setControlVariable(int value, size_t i);
float Tools_Communications_Devices_ADL400_getMeasurementVariable(size_t i);


#endif // !ADL400
