#include "ADL400.h"
#include "../../../Tools.h"
#include <stdint.h>

// Tillfälligt
#include <random>


// Configuration table
std::vector<std::vector<std::string>> configurationTable = { HEADERS_ADL400 };

int readModbusRegister(const char port[], int registerAddress) {
	uint16_t modbusRegister = 0;
	Tools_Hardware_USB_Protocols_Modbus_readRegister(port, registerAddress, 1, &modbusRegister);
	return modbusRegister;
}

std::vector<std::vector<std::string>>& Tools_Communications_Devices_ADL400_getConfigurationTable() {
	return configurationTable;
}

std::string Tools_Communications_Devices_ADL400_getPort(size_t i) {
	return Tools_Communications_Devices_ADL400_getConfigurationTable().at(i+1).at(0);
}

std::string Tools_Communications_Devices_ADL400_getSlaveAddress(size_t i) {
	return Tools_Communications_Devices_ADL400_getConfigurationTable().at(i+1).at(1);
}

std::string Tools_Communications_Devices_ADL400_getFunction(size_t i) {
	return Tools_Communications_Devices_ADL400_getConfigurationTable().at(i+1).at(2);
}

std::string Tools_Communications_Devices_ADL400_getDisplayName(size_t i) {
	return Tools_Communications_Devices_ADL400_getConfigurationTable().at(i+1).at(3);
}

std::vector<std::string> Tools_Communications_Devices_ADL400_getDisplayNames() {
	size_t tableDataSize = Tools_Communications_Devices_ADL400_getConfigurationTableDataSize();
	std::vector<std::string> displayNames;
	for (size_t i = 0; i < tableDataSize; i++) {
		displayNames.push_back(Tools_Communications_Devices_ADL400_getDisplayName(i));
	}
	return displayNames;
}

size_t Tools_Communications_Devices_ADL400_getConfigurationTableDataSize() {
	return Tools_Communications_Devices_ADL400_getConfigurationTable().size() - 1U; // Importat because the functions above are indexed from zero
}

float Tools_Communications_Devices_ADL400_execute(size_t i) {
	const char* port = Tools_Communications_Devices_ADL400_getPort(i).c_str();
	int slaveAddress = std::stoi(Tools_Communications_Devices_ADL400_getSlaveAddress(i));
	std::string function = Tools_Communications_Devices_ADL400_getFunction(i);
	int index = Tools_Software_Algorithms_findIndexOf(FUNCTION_NAMES_ADL400, function);

	// Change slave address
	Tools_Hardware_USB_Protocols_Modbus_setSlaveAddress(port, slaveAddress);

	// Get the measurement now
	float value = 0.0f;
	switch (index) {
	case 0:
		value = ((float)readModbusRegister(port, 0x61)) / 10.0f;
		break;
	case 1:
		value = ((float)readModbusRegister(port, 0x62)) / 10.0f;
		break;
	case 2:
		value = ((float)readModbusRegister(port, 0x63)) / 10.0f;
		break;
	case 3:
		value = ((float)readModbusRegister(port, 0x64)) / 100.0f;
		break;
	case 4:
		value = ((float)readModbusRegister(port, 0x65)) / 100.0f;
		break;
	case 5:
		value = ((float)readModbusRegister(port, 0x66)) / 100.0f;
		break;
	case 6:
		value = ((float)readModbusRegister(port, 0x77)) / 100.0f;
		break;
	case 7:
		value = ((float)readModbusRegister(port, 0x78)) / 10.0f;
		break;
	case 8:
		value = ((float)readModbusRegister(port, 0x79)) / 10.0f;
		break;
	case 9:
		value = ((float)readModbusRegister(port, 0x7A)) / 10.0f;
		break;
	}

	std::random_device rd;
	std::mt19937 generator(rd());

	// Skapa en fördelning för heltal mellan 1 och 100
	std::uniform_int_distribution<int> distribution(1, 100);

	value = distribution(generator);
	return value;
}