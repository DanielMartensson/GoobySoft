#include "OpenSAEJ1939.h"
#include "../../../Tools.h"
#include <stdint.h>

/* Message types for STM32 PLC */
typedef enum {
	SEND_BACK_CAN_MESSAGE_TYPE,
	SEND_BACK_DIGITAL_IN_MESSAGE_TYPE,
	SEND_BACK_ANALOG_IN_MESSAGE_TYPE,
	SEND_BACK_ANALOG_DIFFERENTIAL_IN_MESSAGE_TYPE,
	SEND_BACK_INPUT_CAPUTRE_MESSAGE_TYPE,
	SEND_BACK_ENCODER_MESSAGE_TYPE,
	SEND_BACK_PWM_PRESCALERS_MESSAGE_TYPE,
	SEND_BACK_ANALOG_GAINS_MESSAGE_TYPE,
	SEND_BACK_DATE_TIME_MESSAGE_TYPE,
	SEND_BACK_ALARM_A_MESSAGE_TYPE,
	SEND_BACK_ALARM_B_MESSAGE_TYPE,
	WRITE_SET_CAN_BUS_MESSAGE_TYPE,
	WRITE_SET_PWM_SIGNAL_MESSAGE_TYPE,
	WRITE_SET_DAC_SIGNAL_MESSAGE_TYPE,
	WRITE_SET_ANALOG_INPUT_GAIN_MESSAGE_TYPE,
	WRITE_SET_PWM_PRESCALER_MESSAGE_TYPE,
	WRITE_SET_DATE_TIME_MESSAGE_TYPE,
	WRITE_SET_ALARM_A_MESSAGE_TYPE,
	WRITE_SET_ALARM_B_MESSAGE_TYPE
}MESSAGE_TYPES;

// Configuration table
static std::vector<std::vector<std::string>> configurationTable = { HEADERS_OpenSAEJ1939 };

static uint8_t read8MeasurementByte(const char port[], uint8_t messageType, uint8_t index) {
	uint8_t dataTX[2] = { messageType, index };
	std::vector<uint8_t> dataRX = Tools_Hardware_USB_Protocols_CDC_startTransieveProcesss(port, 10000, dataTX, sizeof(dataTX));
	return !dataRX.empty() ? dataRX.at(0) : 0;
}

static uint16_t read16MeasurementByte(const char port[], uint8_t messageType, uint8_t index) {
	uint8_t dataTX[2] = { messageType, index };
	std::vector<uint8_t> dataRX = Tools_Hardware_USB_Protocols_CDC_startTransieveProcesss(port, 10000, dataTX, sizeof(dataTX));
	if (!dataRX.empty()) {
		return (dataRX.at(0) << 8) | dataRX.at(1);
	}
	else {
		return 0;
	}
}

std::vector<std::vector<std::string>>& Tools_Communications_Devices_OpenSAEJ1939_getConfigurationTable() {
	return configurationTable;
}

std::string Tools_Communications_Devices_OpenSAEJ1939_getPort(size_t i) {
	return Tools_Communications_Devices_OpenSAEJ1939_getConfigurationTable().at(i + 1).at(0);
}

std::string Tools_Communications_Devices_OpenSAEJ1939_getCANAddress(size_t i) {
	return Tools_Communications_Devices_OpenSAEJ1939_getConfigurationTable().at(i + 1).at(1);
}

std::string Tools_Communications_Devices_OpenSAEJ1939_getFunction(size_t i) {
	return Tools_Communications_Devices_OpenSAEJ1939_getConfigurationTable().at(i + 1).at(2);
}

std::string Tools_Communications_Devices_OpenSAEJ1939_getDisplayName(size_t i) {
	return Tools_Communications_Devices_OpenSAEJ1939_getConfigurationTable().at(i + 1).at(3);
}

std::vector<std::string> Tools_Communications_Devices_OpenSAEJ1939_getDisplayNames() {
	size_t tableDataSize = Tools_Communications_Devices_OpenSAEJ1939_getConfigurationTableDataSize();
	std::vector<std::string> displayNames;
	for (size_t i = 0; i < tableDataSize; i++) {
		displayNames.emplace_back(Tools_Communications_Devices_OpenSAEJ1939_getDisplayName(i));
	}
	return displayNames;
}

size_t Tools_Communications_Devices_OpenSAEJ1939_getConfigurationTableDataSize() {
	return Tools_Communications_Devices_OpenSAEJ1939_getConfigurationTable().size() - 1U; // Importat because the functions above are indexed from zero
}

bool Tools_Communications_Devices_OpenSAEJ1939_isInput(size_t i) {
	return true;
}

float* Tools_Communications_Devices_OpenSAEJ1939_getControlVariable(size_t i, float* minValue, float* maxValue) {
	std::string function = Tools_Communications_Devices_OpenSAEJ1939_getFunction(i);
	int index = Tools_Software_Algorithms_findIndexOf(FUNCTION_NAMES_OpenSAEJ1939, function);
	
	static float PWMs[PWM];
	return &PWMs[0]; // TODO: Not done yet
}

float Tools_Communications_Devices_OpenSAEJ1939_setControlVariable(float value, size_t i) {
	return 0;
}

float Tools_Communications_Devices_OpenSAEJ1939_getMeasurementVariable(size_t i) {
	std::string portSTR = Tools_Communications_Devices_OpenSAEJ1939_getPort(i);
	const char* port = portSTR.c_str();
	std::string function = Tools_Communications_Devices_OpenSAEJ1939_getFunction(i);
	int index = Tools_Software_Algorithms_findIndexOf(FUNCTION_NAMES_OpenSAEJ1939, function);


	return 0;
}