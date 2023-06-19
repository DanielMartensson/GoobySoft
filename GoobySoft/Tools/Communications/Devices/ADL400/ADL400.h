#ifndef ADL400_H
#define ADL400_H
#include <string>
#include <vector>

// Constant fields
const std::vector<std::string> HEADERS_ADL400 = { "Port", "Slave address", "Function", "Display name" };
const std::vector<std::string> SLAVE_ADDRESSES_ADL400 = [] {
	std::vector<std::string> slaveAddress;
	for (int i = 1; i <= 254; i++) { slaveAddress.push_back(std::to_string(i)); }
	return slaveAddress;
}();
const std::vector<std::string> FUNCTION_NAMES_ADL400 = { "Read volt A", "Read volt B", "Read volt C", "Read current A", "Read current B", "Read current C", "Read frequency", "Read volt AB", "Read volt AC", "Read volt CB" };


std::vector<std::vector<std::string>>& Tools_Communications_Devices_ADL400_getConfigurationTable();
std::string Tools_Communications_Devices_ADL400_getPort(size_t i);
std::string Tools_Communications_Devices_ADL400_getSlaveAddress(size_t i);
std::string Tools_Communications_Devices_ADL400_getFunction(size_t i);
std::string Tools_Communications_Devices_ADL400_getDisplayName(size_t i);
std::vector<std::string> Tools_Communications_Devices_ADL400_getDisplayNames();
size_t Tools_Communications_Devices_ADL400_getConfigurationTableDataSize();
float Tools_Communications_Devices_ADL400_execute(size_t i);


#endif // !ADL400_H
