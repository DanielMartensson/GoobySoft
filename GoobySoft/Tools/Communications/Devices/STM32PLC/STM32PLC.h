#ifndef STM32PLC
#define STM32PLC
#include <string>
#include <vector>

// Read
#define DIGITAL 10
#define ANALOG 12
#define ANALOG_DIFFERENTIAL 5
#define INPUT_CAPTURE 4
#define ENCODER 3

// Write
#define PWM 8
#define DAC 3

// Constant fields
extern inline const std::vector<std::string> HEADERS_STM32PLC = { "Port", "Function", "Min value raw", "Max value raw", "Min value", "Max value", "Display name" };
extern inline const std::vector<std::string> FUNCTION_NAMES_STM32PLC = [] {
	std::vector<std::string> functionNames;
	for (int i = 0; i < DIGITAL; i++) { functionNames.emplace_back("Read digital " + std::to_string(i)); }
	for (int i = 0; i < ANALOG; i++) { functionNames.emplace_back("Read analog " + std::to_string(i)); }
	for (int i = 0; i < ANALOG_DIFFERENTIAL; i++) { functionNames.emplace_back("Read analog differential " + std::to_string(i)); }
	for (int i = 0; i < INPUT_CAPTURE; i++) { functionNames.emplace_back("Read input capture " + std::to_string(i)); }
	for (int i = 0; i < ENCODER; i++) { functionNames.emplace_back("Read encoder " + std::to_string(i)); }
	for (int i = 0; i < PWM; i++) { functionNames.emplace_back("Write PWM " + std::to_string(i)); }
	for (int i = 0; i < DAC; i++) { functionNames.emplace_back("Write DAC " + std::to_string(i)); }
	return functionNames;
}();

std::vector<std::vector<std::string>>& Tools_Communications_Devices_STM32PLC_getConfigurationTable();
std::string Tools_Communications_Devices_STM32PLC_getPort(size_t i);
std::string Tools_Communications_Devices_STM32PLC_getFunction(size_t i);
std::string Tools_Communications_Devices_STM32PLC_getMinValueRaw(size_t i);
std::string Tools_Communications_Devices_STM32PLC_getMaxValueRaw(size_t i);
std::string Tools_Communications_Devices_STM32PLC_getMinValue(size_t i);
std::string Tools_Communications_Devices_STM32PLC_getMaxValue(size_t i);
std::string Tools_Communications_Devices_STM32PLC_getDisplayName(size_t i);
std::vector<std::string> Tools_Communications_Devices_STM32PLC_getDisplayNames();
size_t Tools_Communications_Devices_STM32PLC_getConfigurationTableDataSize();
bool Tools_Communications_Devices_STM32PLC_isInput(size_t i);
float* Tools_Communications_Devices_STM32PLC_getControlVariable(size_t i, float* minValue, float* maxValue);
float Tools_Communications_Devices_STM32PLC_setControlVariable(float value, size_t i);
float Tools_Communications_Devices_STM32PLC_getMeasurementVariable(size_t i);

#endif // !STM32PLC
