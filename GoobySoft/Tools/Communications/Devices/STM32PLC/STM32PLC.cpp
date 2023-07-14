#include "STM32PLC.h"

// Open SAE J1939 port
static char portOpenSAEJ1939[50] = { 0 };

// Read
#define DIGITAL 10
#define ANALOG 12
#define ANALOG_DIFFERENTIAL 5
#define INPUT_CAPTURE 4
#define ENCODER 3

// Write
#define PWM 8
#define DAC 3
#define AUXILIARY_VALVE_COMMAND 16

/* These must follow the same linear pattern as getFunctionValues() */
typedef enum {
	IO_DIGITAL_INPUT_0,
	IO_DIGITAL_INPUT_1,
	IO_DIGITAL_INPUT_2,
	IO_DIGITAL_INPUT_3,
	IO_DIGITAL_INPUT_4,
	IO_DIGITAL_INPUT_5,
	IO_DIGITAL_INPUT_6,
	IO_DIGITAL_INPUT_7,
	IO_DIGITAL_INPUT_8,
	IO_DIGITAL_INPUT_9,
	IO_ANALOG_INPUT_0,
	IO_ANALOG_INPUT_1,
	IO_ANALOG_INPUT_2,
	IO_ANALOG_INPUT_3,
	IO_ANALOG_INPUT_4,
	IO_ANALOG_INPUT_5,
	IO_ANALOG_INPUT_6,
	IO_ANALOG_INPUT_7,
	IO_ANALOG_INPUT_8,
	IO_ANALOG_INPUT_9,
	IO_ANALOG_INPUT_10,
	IO_ANALOG_INPUT_11,
	IO_ANALOG_DIFF_0,
	IO_ANALOG_DIFF_1,
	IO_ANALOG_DIFF_2,
	IO_ANALOG_DIFF_3,
	IO_ANALOG_DIFF_4,
	IO_INPUT_CAPTURE_0,
	IO_INPUT_CAPTURE_1,
	IO_INPUT_CAPTURE_2,
	IO_INPUT_CAPTURE_3,
	IO_ENCODER_0,
	IO_ENCODER_1,
	IO_ENCODER_2,
	IO_PWM_0,
	IO_PWM_1,
	IO_PWM_2,
	IO_PWM_3,
	IO_PWM_4,
	IO_PWM_5,
	IO_PWM_6,
	IO_PWM_7,
	IO_DAC_0,
	IO_DAC_1,
	IO_DAC_2,
	IO_AUXILIARY_VALVE_0,
	IO_AUXILIARY_VALVE_1,
	IO_AUXILIARY_VALVE_2,
	IO_AUXILIARY_VALVE_3,
	IO_AUXILIARY_VALVE_4,
	IO_AUXILIARY_VALVE_5,
	IO_AUXILIARY_VALVE_6,
	IO_AUXILIARY_VALVE_7,
	IO_AUXILIARY_VALVE_8,
	IO_AUXILIARY_VALVE_9,
	IO_AUXILIARY_VALVE_10,
	IO_AUXILIARY_VALVE_11,
	IO_AUXILIARY_VALVE_12,
	IO_AUXILIARY_VALVE_13,
	IO_AUXILIARY_VALVE_14,
	IO_AUXILIARY_VALVE_15
}IO;

static uint8_t readInputUint8_t(const char port[], uint8_t messageType, uint8_t index) {
	uint8_t dataTX[2] = { messageType, index };
	std::vector<uint8_t> dataRX = Tools_Hardware_USB_Protocols_CDC_startTransieveProcesss(port, 1000, dataTX, sizeof(dataTX));
	return !dataRX.empty() ? dataRX.at(0) : 0;
}

static uint16_t readInputUint16_t(const char port[], uint8_t messageType, uint8_t index) {
	uint8_t dataTX[2] = { messageType, index };
	std::vector<uint8_t> dataRX = Tools_Hardware_USB_Protocols_CDC_startTransieveProcesss(port, 1000, dataTX, sizeof(dataTX));
	return !dataRX.empty() ? (dataRX.at(0) << 8) | dataRX.at(1) : 0;
}

static bool writeOutputUint16_t(const char port[], uint8_t messageType, uint8_t index, uint16_t value) {
	uint8_t dataTX[4] = { messageType, index, value >> 8, value };
	std::vector<uint8_t> dataRX = Tools_Hardware_USB_Protocols_CDC_startTransieveProcesss(port, 1000, dataTX, sizeof(dataTX));
	return !dataRX.empty();
}

static bool writeSAEJ1939AuxiliaryValveCommand(const char port[], uint8_t valve_number, uint8_t standard_flow) {
	// Get the J1939 struct
	J1939* j1939 = Tools_Hardware_ParameterStore_getJ1939Holder();

	// Add the valve states
	uint8_t valve_state = VALVE_STATE_NEUTRAL;
	if (standard_flow > 0) {
		valve_state = VALVE_STATE_EXTEND;
	}
	if (standard_flow < 0) {
		valve_state = VALVE_STATE_RETRACT;
	}

	// Save the port 
	std::strcpy(portOpenSAEJ1939, port);

	// Write the command
	ENUM_J1939_STATUS_CODES status = ISO_11783_Send_Auxiliary_Valve_Command(j1939, valve_number, standard_flow, 0, valve_state);
	return status == STATUS_SEND_OK;
}

std::vector<TableColumnID> Tools_Communications_Devices_STM32PLC_getTableColumnIDs() {
	// Only one column definition is allowed.
	std::vector<TableColumnID> tableColumnIDs;
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Port", COLUMN_DEFINITION_PORT)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Function", COLUMN_DEFINITION_FUNCTION)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("CAN address", COLUMN_DEFINITION_ADDRESS));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Min value raw", COLUMN_DEFINITION_MIN_VALUE_RAW));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Max value raw", COLUMN_DEFINITION_MAX_VALUE_RAW));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Min value real", COLUMN_DEFINITION_MIN_VALUE_REAL));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Max value real", COLUMN_DEFINITION_MAX_VALUE_REAL));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Display name", COLUMN_DEFINITION_DISPLAY_NAME)); // Mandatory field
	return tableColumnIDs;
}

COLUMN_FUNCTION Tools_Communications_Devices_STM32PLC_getColumnFunction(int functionValueIndex) {
	/* These must follow the same linear pattern as getFunctionValues() */
	switch (functionValueIndex) {
	case IO_DIGITAL_INPUT_0:
		return COLUMN_FUNCTION_INPUT_SENSOR_NO_CALIBRATION;
	case IO_DIGITAL_INPUT_1:
		return COLUMN_FUNCTION_INPUT_SENSOR_NO_CALIBRATION;
	case IO_DIGITAL_INPUT_2:
		return COLUMN_FUNCTION_INPUT_SENSOR_NO_CALIBRATION;
	case IO_DIGITAL_INPUT_3:
		return COLUMN_FUNCTION_INPUT_SENSOR_NO_CALIBRATION;
	case IO_DIGITAL_INPUT_4:
		return COLUMN_FUNCTION_INPUT_SENSOR_NO_CALIBRATION;
	case IO_DIGITAL_INPUT_5:
		return COLUMN_FUNCTION_INPUT_SENSOR_NO_CALIBRATION;
	case IO_DIGITAL_INPUT_6:
		return COLUMN_FUNCTION_INPUT_SENSOR_NO_CALIBRATION;
	case IO_DIGITAL_INPUT_7:
		return COLUMN_FUNCTION_INPUT_SENSOR_NO_CALIBRATION;
	case IO_DIGITAL_INPUT_8:
		return COLUMN_FUNCTION_INPUT_SENSOR_NO_CALIBRATION;
	case IO_DIGITAL_INPUT_9:
		return COLUMN_FUNCTION_INPUT_SENSOR_NO_CALIBRATION;
	case IO_ANALOG_INPUT_0:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_ANALOG_INPUT_1:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_ANALOG_INPUT_2:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_ANALOG_INPUT_3:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_ANALOG_INPUT_4:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_ANALOG_INPUT_5:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_ANALOG_INPUT_6:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_ANALOG_INPUT_7:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_ANALOG_INPUT_8:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_ANALOG_INPUT_9:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_ANALOG_INPUT_10:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_ANALOG_INPUT_11:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_ANALOG_DIFF_0:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_ANALOG_DIFF_1:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_ANALOG_DIFF_2:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_ANALOG_DIFF_3:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_ANALOG_DIFF_4:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_INPUT_CAPTURE_0:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_INPUT_CAPTURE_1:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_INPUT_CAPTURE_2:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_INPUT_CAPTURE_3:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_ENCODER_0:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_ENCODER_1:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_ENCODER_2:
		return COLUMN_FUNCTION_INPUT_SENSOR_ANALOG;
	case IO_PWM_0:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_PWM_1:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_PWM_2:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_PWM_3:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_PWM_4:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_PWM_5:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_PWM_6:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_PWM_7:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_DAC_0:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_DAC_1:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_DAC_2:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_AUXILIARY_VALVE_0:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_AUXILIARY_VALVE_1:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_AUXILIARY_VALVE_2:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_AUXILIARY_VALVE_3:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_AUXILIARY_VALVE_4:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_AUXILIARY_VALVE_5:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_AUXILIARY_VALVE_6:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_AUXILIARY_VALVE_7:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_AUXILIARY_VALVE_8:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_AUXILIARY_VALVE_9:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_AUXILIARY_VALVE_10:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_AUXILIARY_VALVE_11:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_AUXILIARY_VALVE_12:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_AUXILIARY_VALVE_13:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_AUXILIARY_VALVE_14:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	case IO_AUXILIARY_VALVE_15:
		return COLUMN_FUNCTION_OUTPUT_ACTUATOR;
	default:
		return COLUMN_FUNCTION_HOLD_DATA;
	}
}

std::string Tools_Communications_Devices_STM32PLC_getFunctionValues() {
	std::string functionNames;
	for (int i = 0; i < DIGITAL; i++) { 
		functionNames += "Digital " + std::to_string(i);
		functionNames += '\0';
	}
	for (int i = 0; i < ANALOG; i++) { 
		functionNames += "Analog " + std::to_string(i);
		functionNames += '\0';
	}
	for (int i = 0; i < ANALOG_DIFFERENTIAL; i++) { 
		functionNames += "Analog differential " + std::to_string(i);
		functionNames += '\0';
	}
	for (int i = 0; i < INPUT_CAPTURE; i++) { 
		functionNames += "Input capture " + std::to_string(i);
		functionNames += '\0';
	}
	for (int i = 0; i < ENCODER; i++) { 
		functionNames += "Encoder " + std::to_string(i);
		functionNames += '\0';
	}
	for (int i = 0; i < PWM; i++) {
		functionNames += "PWM " + std::to_string(i);
		functionNames += '\0';
	}
	for (int i = 0; i < DAC; i++) { 
		functionNames += "DAC " + std::to_string(i);
		functionNames += '\0';
	}
	for (int i = 0; i < AUXILIARY_VALVE_COMMAND; i++) {
		functionNames += "AUX valve " + std::to_string(i);
		functionNames += '\0';
	}
	return functionNames;
}

float Tools_Communications_Devices_STM32PLC_getInput(const char port[], int functionValueIndex, int address) {
	/* These must follow the same linear pattern as getFunctionValues() */
	switch (functionValueIndex) {
	case IO_DIGITAL_INPUT_0:
		return readInputUint8_t(port, SEND_BACK_DIGITAL_IN_MESSAGE_TYPE, 0);
	case IO_DIGITAL_INPUT_1:
		return readInputUint8_t(port, SEND_BACK_DIGITAL_IN_MESSAGE_TYPE, 1);
	case IO_DIGITAL_INPUT_2:
		return readInputUint8_t(port, SEND_BACK_DIGITAL_IN_MESSAGE_TYPE, 2);
	case IO_DIGITAL_INPUT_3:
		return readInputUint8_t(port, SEND_BACK_DIGITAL_IN_MESSAGE_TYPE, 3);
	case IO_DIGITAL_INPUT_4:
		return readInputUint8_t(port, SEND_BACK_DIGITAL_IN_MESSAGE_TYPE, 4);
	case IO_DIGITAL_INPUT_5:
		return readInputUint8_t(port, SEND_BACK_DIGITAL_IN_MESSAGE_TYPE, 5);
	case IO_DIGITAL_INPUT_6:
		return readInputUint8_t(port, SEND_BACK_DIGITAL_IN_MESSAGE_TYPE, 6);
	case IO_DIGITAL_INPUT_7:
		return readInputUint8_t(port, SEND_BACK_DIGITAL_IN_MESSAGE_TYPE, 7);
	case IO_DIGITAL_INPUT_8:
		return readInputUint8_t(port, SEND_BACK_DIGITAL_IN_MESSAGE_TYPE, 8);
	case IO_DIGITAL_INPUT_9:
		return readInputUint8_t(port, SEND_BACK_DIGITAL_IN_MESSAGE_TYPE, 9);
	case IO_ANALOG_INPUT_0:
		return readInputUint16_t(port, SEND_BACK_ANALOG_IN_MESSAGE_TYPE, 0);
	case IO_ANALOG_INPUT_1:
		return readInputUint16_t(port, SEND_BACK_ANALOG_IN_MESSAGE_TYPE, 1);
	case IO_ANALOG_INPUT_2:
		return readInputUint16_t(port, SEND_BACK_ANALOG_IN_MESSAGE_TYPE, 2);
	case IO_ANALOG_INPUT_3:
		return readInputUint16_t(port, SEND_BACK_ANALOG_IN_MESSAGE_TYPE, 3);
	case IO_ANALOG_INPUT_4:
		return readInputUint16_t(port, SEND_BACK_ANALOG_IN_MESSAGE_TYPE, 4);
	case IO_ANALOG_INPUT_5:
		return readInputUint16_t(port, SEND_BACK_ANALOG_IN_MESSAGE_TYPE, 5);
	case IO_ANALOG_INPUT_6:
		return readInputUint16_t(port, SEND_BACK_ANALOG_IN_MESSAGE_TYPE, 6);
	case IO_ANALOG_INPUT_7:
		return readInputUint16_t(port, SEND_BACK_ANALOG_IN_MESSAGE_TYPE, 7);
	case IO_ANALOG_INPUT_8:
		return readInputUint16_t(port, SEND_BACK_ANALOG_IN_MESSAGE_TYPE, 8);
	case IO_ANALOG_INPUT_9:
		return readInputUint16_t(port, SEND_BACK_ANALOG_IN_MESSAGE_TYPE, 9);
	case IO_ANALOG_INPUT_10:
		return readInputUint16_t(port, SEND_BACK_ANALOG_IN_MESSAGE_TYPE, 10);
	case IO_ANALOG_INPUT_11:
		return readInputUint16_t(port, SEND_BACK_ANALOG_IN_MESSAGE_TYPE, 11);
	case IO_ANALOG_DIFF_0:
		return readInputUint16_t(port, SEND_BACK_ANALOG_DIFFERENTIAL_IN_MESSAGE_TYPE, 0);
	case IO_ANALOG_DIFF_1:
		return readInputUint16_t(port, SEND_BACK_ANALOG_DIFFERENTIAL_IN_MESSAGE_TYPE, 1);
	case IO_ANALOG_DIFF_2:
		return readInputUint16_t(port, SEND_BACK_ANALOG_DIFFERENTIAL_IN_MESSAGE_TYPE, 2);
	case IO_ANALOG_DIFF_3:
		return readInputUint16_t(port, SEND_BACK_ANALOG_DIFFERENTIAL_IN_MESSAGE_TYPE, 3);
	case IO_ANALOG_DIFF_4:
		return readInputUint16_t(port, SEND_BACK_ANALOG_DIFFERENTIAL_IN_MESSAGE_TYPE, 4);
	case IO_INPUT_CAPTURE_0:
		return 1.0f/(0.0001f * ((float)readInputUint16_t(port, SEND_BACK_INPUT_CAPUTRE_MESSAGE_TYPE, 0))); // Hz
	case IO_INPUT_CAPTURE_1:
		return 1.0f / (0.0001f * ((float)readInputUint16_t(port, SEND_BACK_INPUT_CAPUTRE_MESSAGE_TYPE, 1))); // Hz
	case IO_INPUT_CAPTURE_2:
		return 1.0f / (0.0001f * ((float)readInputUint16_t(port, SEND_BACK_INPUT_CAPUTRE_MESSAGE_TYPE, 2))); // Hz
	case IO_INPUT_CAPTURE_3:
		return 1.0f / (0.0001f * ((float)readInputUint16_t(port, SEND_BACK_INPUT_CAPUTRE_MESSAGE_TYPE, 3))); // Hz
	case IO_ENCODER_0:
		return readInputUint16_t(port, SEND_BACK_ENCODER_MESSAGE_TYPE, 0);
	case IO_ENCODER_1:
		return readInputUint16_t(port, SEND_BACK_ENCODER_MESSAGE_TYPE, 1);
	case IO_ENCODER_2:
		return readInputUint16_t(port, SEND_BACK_ENCODER_MESSAGE_TYPE, 2);
	default:
		return -1.0f;
	}
}

bool Tools_Communications_Devices_STM32PLC_setOutput(const char port[], int functionValueIndex, int address, int value) {
	/* These must follow the same linear pattern as getFunctionValues() */
	switch (functionValueIndex) {
	case IO_PWM_0:
		return writeOutputUint16_t(port, WRITE_SET_PWM_SIGNAL_MESSAGE_TYPE, 0, value);
	case IO_PWM_1:
		return writeOutputUint16_t(port, WRITE_SET_PWM_SIGNAL_MESSAGE_TYPE, 1, value);
	case IO_PWM_2:
		return writeOutputUint16_t(port, WRITE_SET_PWM_SIGNAL_MESSAGE_TYPE, 2, value);
	case IO_PWM_3:
		return writeOutputUint16_t(port, WRITE_SET_PWM_SIGNAL_MESSAGE_TYPE, 3, value);
	case IO_PWM_4:
		return writeOutputUint16_t(port, WRITE_SET_PWM_SIGNAL_MESSAGE_TYPE, 4, value);
	case IO_PWM_5:
		return writeOutputUint16_t(port, WRITE_SET_PWM_SIGNAL_MESSAGE_TYPE, 5, value);
	case IO_PWM_6:
		return writeOutputUint16_t(port, WRITE_SET_PWM_SIGNAL_MESSAGE_TYPE, 6, value);
	case IO_PWM_7:
		return writeOutputUint16_t(port, WRITE_SET_PWM_SIGNAL_MESSAGE_TYPE, 7, value);
	case IO_DAC_0:
		return writeOutputUint16_t(port, WRITE_SET_DAC_SIGNAL_MESSAGE_TYPE, 0, value);
	case IO_DAC_1:
		return writeOutputUint16_t(port, WRITE_SET_DAC_SIGNAL_MESSAGE_TYPE, 1, value);
	case IO_DAC_2:
		return writeOutputUint16_t(port, WRITE_SET_DAC_SIGNAL_MESSAGE_TYPE, 2, value);
	case IO_AUXILIARY_VALVE_0:
		return writeSAEJ1939AuxiliaryValveCommand(port, 0, value);
	case IO_AUXILIARY_VALVE_1:
		return writeSAEJ1939AuxiliaryValveCommand(port, 1, value);
	case IO_AUXILIARY_VALVE_2:
		return writeSAEJ1939AuxiliaryValveCommand(port, 2, value);
	case IO_AUXILIARY_VALVE_3:
		return writeSAEJ1939AuxiliaryValveCommand(port, 3, value);
	case IO_AUXILIARY_VALVE_4:
		return writeSAEJ1939AuxiliaryValveCommand(port, 4, value);
	case IO_AUXILIARY_VALVE_5:
		return writeSAEJ1939AuxiliaryValveCommand(port, 5, value);
	case IO_AUXILIARY_VALVE_6:
		return writeSAEJ1939AuxiliaryValveCommand(port, 6, value);
	case IO_AUXILIARY_VALVE_7:
		return writeSAEJ1939AuxiliaryValveCommand(port, 7, value);
	case IO_AUXILIARY_VALVE_8:
		return writeSAEJ1939AuxiliaryValveCommand(port, 8, value);
	case IO_AUXILIARY_VALVE_9:
		return writeSAEJ1939AuxiliaryValveCommand(port, 9, value);
	case IO_AUXILIARY_VALVE_10:
		return writeSAEJ1939AuxiliaryValveCommand(port, 10, value);
	case IO_AUXILIARY_VALVE_11:
		return writeSAEJ1939AuxiliaryValveCommand(port, 11, value);
	case IO_AUXILIARY_VALVE_12:
		return writeSAEJ1939AuxiliaryValveCommand(port, 12, value);
	case IO_AUXILIARY_VALVE_13:
		return writeSAEJ1939AuxiliaryValveCommand(port, 13, value);
	case IO_AUXILIARY_VALVE_14:
		return writeSAEJ1939AuxiliaryValveCommand(port, 14, value);
	case IO_AUXILIARY_VALVE_15:
		return writeSAEJ1939AuxiliaryValveCommand(port, 15, value);
	default:
		return false;
	}
}

const char* Tools_Communications_Devices_STM32PLC_getPortOpenSAEJ1939() {
	return portOpenSAEJ1939;
}