#include "STM32PLC.h"
#include "../../../Tools.h"
#include <stdint.h>
#include <cmath>

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
	WRITE_SET_ALARM_B_MESSAGE_TYPE,
	ACKNOWLEDGEMENT_MESSAGE_TYPE
}MESSAGE_TYPES;

// Configuration table
static std::vector<std::vector<std::string>> configurationTable = { HEADERS_STM32PLC };

static uint8_t read8MeasurementByte(const char port[], uint8_t messageType, uint8_t index) {
	uint8_t dataTX[2] = { messageType, index };
	std::vector<uint8_t> dataRX = Tools_Hardware_USB_Protocols_CDC_startTransieveProcesss(port, 1000, dataTX, sizeof(dataTX));
	return !dataRX.empty() ? dataRX.at(0) : 0;
}

static uint16_t read16MeasurementByte(const char port[], uint8_t messageType, uint8_t index) {
	uint8_t dataTX[2] = { messageType, index };
	std::vector<uint8_t> dataRX = Tools_Hardware_USB_Protocols_CDC_startTransieveProcesss(port, 1000, dataTX, sizeof(dataTX));
	if (!dataRX.empty()) {
		return (dataRX.at(0) << 8) | dataRX.at(1);
	}
	else {
		return 0;
	}
}

static bool write16ControlByte(const char port[], uint8_t messageType, uint8_t index, uint16_t value) {
	uint8_t dataTX[4] = { messageType, index, value >> 8, value };
	std::vector<uint8_t> dataRX = Tools_Hardware_USB_Protocols_CDC_startTransieveProcesss(port, 1000, dataTX, sizeof(dataTX));
	return !dataRX.empty();
}

std::vector<std::vector<std::string>>& Tools_Communications_Devices_STM32PLC_getConfigurationTable() {
	return configurationTable;
}

std::string Tools_Communications_Devices_STM32PLC_getPort(size_t i) {
	return Tools_Communications_Devices_STM32PLC_getConfigurationTable().at(i + 1).at(0);
}

std::string Tools_Communications_Devices_STM32PLC_getFunction(size_t i) {
	return Tools_Communications_Devices_STM32PLC_getConfigurationTable().at(i + 1).at(1);
}

std::string Tools_Communications_Devices_STM32PLC_getMinValueRaw(size_t i) {
	return Tools_Communications_Devices_STM32PLC_getConfigurationTable().at(i + 1).at(2);
}

std::string Tools_Communications_Devices_STM32PLC_getMaxValueRaw(size_t i) {
	return Tools_Communications_Devices_STM32PLC_getConfigurationTable().at(i + 1).at(3);
}

std::string Tools_Communications_Devices_STM32PLC_getMinValue(size_t i) {
	return Tools_Communications_Devices_STM32PLC_getConfigurationTable().at(i + 1).at(4);
}

std::string Tools_Communications_Devices_STM32PLC_getMaxValue(size_t i) {
	return Tools_Communications_Devices_STM32PLC_getConfigurationTable().at(i + 1).at(5);
}

std::string Tools_Communications_Devices_STM32PLC_getDisplayName(size_t i) {
	return Tools_Communications_Devices_STM32PLC_getConfigurationTable().at(i + 1).at(6);
}

std::vector<std::string> Tools_Communications_Devices_STM32PLC_getDisplayNames() {
	size_t tableDataSize = Tools_Communications_Devices_STM32PLC_getConfigurationTableDataSize();
	std::vector<std::string> displayNames;
	for (size_t i = 0; i < tableDataSize; i++) {
		displayNames.emplace_back(Tools_Communications_Devices_STM32PLC_getDisplayName(i));
	}
	return displayNames;
}

size_t Tools_Communications_Devices_STM32PLC_getConfigurationTableDataSize() {
	return Tools_Communications_Devices_STM32PLC_getConfigurationTable().size() - 1U; // Importat because the functions above are indexed from zero
}

bool Tools_Communications_Devices_STM32PLC_isInput(size_t i) {
	std::string function = Tools_Communications_Devices_STM32PLC_getFunction(i);
	int index = Tools_Software_Algorithms_findIndexOf(FUNCTION_NAMES_STM32PLC, function);
	return index < DIGITAL + ANALOG + ANALOG_DIFFERENTIAL + INPUT_CAPTURE + ENCODER ? true : false;
}

float* Tools_Communications_Devices_STM32PLC_getControlVariable(size_t i, float* minValue, float* maxValue) {
	std::string function = Tools_Communications_Devices_STM32PLC_getFunction(i);
	int index = Tools_Software_Algorithms_findIndexOf(FUNCTION_NAMES_STM32PLC, function);
	*minValue = std::stof(Tools_Communications_Devices_STM32PLC_getMinValue(i));
	*maxValue = std::stof(Tools_Communications_Devices_STM32PLC_getMaxValue(i));
	
	// Determine if PWM or DAC
	static float PWM_values[PWM] = { 0 };
	static float DAC_values[DAC] = { 0 };
	if (index >= DIGITAL + ANALOG + ANALOG_DIFFERENTIAL + INPUT_CAPTURE + ENCODER && index < DIGITAL + ANALOG + ANALOG_DIFFERENTIAL + INPUT_CAPTURE + ENCODER + PWM) {
		return &PWM_values[index - DIGITAL - ANALOG - ANALOG_DIFFERENTIAL - INPUT_CAPTURE - ENCODER];
	}
	else if (index >= DIGITAL + ANALOG + ANALOG_DIFFERENTIAL + INPUT_CAPTURE + ENCODER + PWM && index < DIGITAL + ANALOG + ANALOG_DIFFERENTIAL + INPUT_CAPTURE + ENCODER + PWM + DAC) {
		return &DAC_values[index - DIGITAL - ANALOG - ANALOG_DIFFERENTIAL - INPUT_CAPTURE - ENCODER - PWM];
	}
	else {
		return &PWM_values[0];
	}
}

float Tools_Communications_Devices_STM32PLC_setControlVariable(float value, size_t i) {
	std::string portSTR = Tools_Communications_Devices_STM32PLC_getPort(i);
	const char* port = portSTR.c_str();
	std::string function = Tools_Communications_Devices_STM32PLC_getFunction(i);
	int index = Tools_Software_Algorithms_findIndexOf(FUNCTION_NAMES_STM32PLC, function);

	// Calibration
	float minValueRaw = std::stof(Tools_Communications_Devices_STM32PLC_getMinValueRaw(i));
	float maxValueRaw = std::stof(Tools_Communications_Devices_STM32PLC_getMaxValueRaw(i));
	float minValue = std::stof(Tools_Communications_Devices_STM32PLC_getMinValue(i));
	float maxValue = std::stof(Tools_Communications_Devices_STM32PLC_getMaxValue(i));

	uint16_t value16Bit = 0;
	float y = 0;
	
	// Determine if PWM or DAC
	if (index >= DIGITAL + ANALOG + ANALOG_DIFFERENTIAL + INPUT_CAPTURE + ENCODER && index < DIGITAL + ANALOG + ANALOG_DIFFERENTIAL + INPUT_CAPTURE + ENCODER + PWM) {
		// Make value absolute
		value = std::abs(value);
		value16Bit = 65535.0f / (maxValue - minValue) * value;
		write16ControlByte(port, WRITE_SET_PWM_SIGNAL_MESSAGE_TYPE, index - DIGITAL - ANALOG - ANALOG_DIFFERENTIAL - INPUT_CAPTURE - ENCODER, value16Bit);
	}
	else if (index >= DIGITAL + ANALOG + ANALOG_DIFFERENTIAL + INPUT_CAPTURE + ENCODER + PWM && index < DIGITAL + ANALOG + ANALOG_DIFFERENTIAL + INPUT_CAPTURE + ENCODER + PWM + DAC) {
		// Make value absolute
		value = std::abs(value);
		value16Bit = 4095.0f / (maxValue - minValue) * value;
		write16ControlByte(port, WRITE_SET_DAC_SIGNAL_MESSAGE_TYPE, index - DIGITAL - ANALOG - ANALOG_DIFFERENTIAL - INPUT_CAPTURE - ENCODER - PWM, value16Bit);
	}

	// Slope equation
	float k = (maxValue - minValue) / (maxValueRaw - minValueRaw);
	float m = maxValue - k * maxValueRaw;
	y = k * value + m;
	return y;
}

float Tools_Communications_Devices_STM32PLC_getMeasurementVariable(size_t i) {
	std::string portSTR = Tools_Communications_Devices_STM32PLC_getPort(i);
	const char* port = portSTR.c_str();
	std::string function = Tools_Communications_Devices_STM32PLC_getFunction(i);
	int index = Tools_Software_Algorithms_findIndexOf(FUNCTION_NAMES_STM32PLC, function);

	// Value
	float value = 0;

	// Inputs
	if (index < DIGITAL) {
		value = read8MeasurementByte(port, SEND_BACK_DIGITAL_IN_MESSAGE_TYPE, index);
	}
	else if (index >= DIGITAL && index < DIGITAL + ANALOG) {
		value = read16MeasurementByte(port, SEND_BACK_ANALOG_IN_MESSAGE_TYPE, index - DIGITAL);
	}
	else if (index >= DIGITAL + ANALOG && index < DIGITAL + ANALOG + ANALOG_DIFFERENTIAL) {
		value = read16MeasurementByte(port, SEND_BACK_ANALOG_DIFFERENTIAL_IN_MESSAGE_TYPE, index - DIGITAL - ANALOG);
	}
	else if (index >= DIGITAL + ANALOG + ANALOG_DIFFERENTIAL && index < DIGITAL + ANALOG + ANALOG_DIFFERENTIAL + INPUT_CAPTURE) {
		value = read16MeasurementByte(port, SEND_BACK_INPUT_CAPUTRE_MESSAGE_TYPE, index - DIGITAL - ANALOG - ANALOG_DIFFERENTIAL);
	}
	else if (index >= DIGITAL + ANALOG + ANALOG_DIFFERENTIAL + INPUT_CAPTURE && index < DIGITAL + ANALOG + ANALOG_DIFFERENTIAL + INPUT_CAPTURE + ENCODER) {
		value = read16MeasurementByte(port, SEND_BACK_ENCODER_MESSAGE_TYPE, index - DIGITAL - ANALOG - ANALOG_DIFFERENTIAL - INPUT_CAPTURE);
	}

	// Calibration
	float minValueRaw = std::stof(Tools_Communications_Devices_STM32PLC_getMinValueRaw(i));
	float maxValueRaw = std::stof(Tools_Communications_Devices_STM32PLC_getMaxValueRaw(i));
	float minValue = std::stof(Tools_Communications_Devices_STM32PLC_getMinValue(i));
	float maxValue = std::stof(Tools_Communications_Devices_STM32PLC_getMaxValue(i));

	// This is a special case for imput capture
	if (index >= DIGITAL + ANALOG + ANALOG_DIFFERENTIAL && index < DIGITAL + ANALOG + ANALOG_DIFFERENTIAL + INPUT_CAPTURE) {
		value = 1 / (value * 0.0001f); // 0.1526 Hz to 10kHz
	}

	// Slope equation
	float k = (maxValue - minValue) / (maxValueRaw - minValueRaw);
	float m = maxValue - k * maxValueRaw;
	float y = k * value + m;
	return y;
}