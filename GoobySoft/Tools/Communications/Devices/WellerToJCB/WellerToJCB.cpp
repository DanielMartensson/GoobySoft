#include "WellerToJBC.h"

/* These must follow the same linear pattern as getFunctionValues() */
typedef enum {
	IO_READ_CURRENT_RAW,
	IO_READ_CURRENT_REAL,
	IO_READ_TEMPERATURE_RAW,
	IO_READ_TEMPERATURE_REAL,
	IO_READ_SETPOINT_RAW,
	IO_READ_SETPOINT_REAL,
	IO_READ_STATE_XHAT,
	IO_READ_LED_GREEN,
	IO_READ_LEACKAGE_ACTIVE,
	IO_READ_SLEEP,
	IO_READ_ITERATION_TIME_MS
}IO;

std::string Tools_Communications_Devices_WellerToJBC_getFunctionValues() {
	std::string functionNames;
	functionNames += "Read Current Raw";
	functionNames += '\0';
	functionNames += "Read Current Real";
	functionNames += '\0';
	functionNames += "Read Temperature Raw";
	functionNames += '\0';
	functionNames += "Read Temperature Real";
	functionNames += '\0';
	functionNames += "Read Setpoint Raw";
	functionNames += '\0';
	functionNames += "Read Setpoint Real";
	functionNames += '\0';
	functionNames += "Read estimated state x";
	functionNames += '\0';
	functionNames += "Read LED green";
	functionNames += '\0';
	functionNames += "Read leackage active";
	functionNames += '\0';
	functionNames += "Read sleep active";
	functionNames += '\0';
	functionNames += "Read iteration time ms";
	functionNames += '\0';
	return functionNames;
}
std::vector<TableColumnID> Tools_Communications_Devices_WellerToJBC_getTableColumnIDs() {
	std::vector<TableColumnID> tableColumnIDs;
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Port", COLUMN_DEFINITION_PORT)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Function", COLUMN_DEFINITION_FUNCTION)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Modbus address", COLUMN_DEFINITION_ADDRESS));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Min value raw", COLUMN_DEFINITION_MIN_VALUE_RAW));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Max value raw", COLUMN_DEFINITION_MAX_VALUE_RAW));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Min value real", COLUMN_DEFINITION_MIN_VALUE_REAL));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Max value real", COLUMN_DEFINITION_MAX_VALUE_REAL));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Display name", COLUMN_DEFINITION_DISPLAY_NAME)); // Mandatory field
	return tableColumnIDs;
}
float Tools_Communications_Devices_WellerToJBC_getInput(const char port[], int functionValueIndex, int address) {
	/* These must follow the same linear pattern as getFunctionValues() */
	modbus_client_set_RTU_address(address);
	modbus_set_serial_port(port);
	uint16_t registers[2];
	uint8_t digitals[1];
	float value;
	switch (functionValueIndex) {
	case IO_READ_CURRENT_RAW:
		modbus_client_get_analog_inputs(registers, 2, 1);
		return (float)registers[0];
	case IO_READ_CURRENT_REAL:
		modbus_client_get_analog_inputs(registers, 7, 2);
		return uint16_to_float(registers[1], registers[0]);
	case IO_READ_TEMPERATURE_RAW:
		modbus_client_get_analog_inputs(registers, 0, 1);
		return (float) registers[0];
	case IO_READ_TEMPERATURE_REAL:
		modbus_client_get_analog_inputs(registers, 3, 2);
		return uint16_to_float(registers[1], registers[0]);
	case IO_READ_SETPOINT_RAW:
		modbus_client_get_analog_inputs(registers, 1, 1);
		return (float)registers[0];
	case IO_READ_SETPOINT_REAL:
		modbus_client_get_analog_inputs(registers, 5, 2);
		value = uint16_to_float(registers[1], registers[0]);
		return value;
	case IO_READ_STATE_XHAT:
		modbus_client_get_analog_inputs(registers, 9, 2);
		return uint16_to_float(registers[1], registers[0]);
	case IO_READ_LED_GREEN:
		modbus_client_get_digital_outputs(digitals, 1, 1);
		return (float)digitals[0];
	case IO_READ_LEACKAGE_ACTIVE:
		modbus_client_get_digital_outputs(digitals, 0, 1);
		return (float)digitals[0];
	case IO_READ_SLEEP:
		modbus_client_get_digital_inputs(digitals, 0, 1);
		return (float)digitals[0];
	case IO_READ_ITERATION_TIME_MS:
		modbus_client_get_analog_inputs(registers, 11, 1);
		return registers[0];
	default:
		return -1.0f;
	}
}

bool Tools_Communications_Devices_WellerToJBC_setOutput(const char port[], int functionValueIndex, int address, int value) {
	switch (functionValueIndex) {
	default:
		return false; // There are no outputs for WellerToJCB
	}
}

COLUMN_FUNCTION Tools_Communications_Devices_WellerToJBC_getColumnFunction(int functionValueIndex){
	/* These must follow the same linear pattern as getFunctionValues() */
	switch (functionValueIndex) {
	case IO_READ_CURRENT_RAW:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS_NO_CALIBRATION;
	case IO_READ_CURRENT_REAL:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS_NO_CALIBRATION;
	case IO_READ_TEMPERATURE_RAW:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS_NO_CALIBRATION;
	case IO_READ_TEMPERATURE_REAL:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS_NO_CALIBRATION;
	case IO_READ_SETPOINT_RAW:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS_NO_CALIBRATION;
	case IO_READ_SETPOINT_REAL:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS_NO_CALIBRATION;
	case IO_READ_STATE_XHAT:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS_NO_CALIBRATION;
	case IO_READ_LED_GREEN:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS_NO_CALIBRATION;
	case IO_READ_LEACKAGE_ACTIVE:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS_NO_CALIBRATION;
	case IO_READ_SLEEP:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS_NO_CALIBRATION;
	case IO_READ_ITERATION_TIME_MS:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS_NO_CALIBRATION;
	default:
		return COLUMN_FUNCTION_HOLD_DATA; // Just hold data
	}
}

bool Tools_Communications_Devices_WellerToJBC_setAnalogInputCalibration(const char port[], const int address, const int rawMin[], const int rawMax[], const float realMin[], const float realMax[]) {
	modbus_client_set_RTU_address(address);
	modbus_set_serial_port(port);
	uint16_t registers[18];
	for (int i = 0; i < 3; i++) {
		registers[6 * i + 0] = rawMin[i];
		registers[6 * i + 1] = rawMax[i];
		float_to_uint16(realMin[i], &registers[6 * i + 3], &registers[6 * i + 2]);
		float_to_uint16(realMax[i], &registers[6 * i + 5], &registers[6 * i + 4]);
	}
	const bool status = modbus_client_set_parameters(registers, 0, 18);
	return status;
}

bool Tools_Communications_Devices_WellerToJBC_getAnalogInputCalibration(const char port[], const int address, int rawMin[], int rawMax[], float realMin[], float realMax[]) {
	modbus_client_set_RTU_address(address);
	modbus_set_serial_port(port);
	uint16_t registers[18];
	const bool status = modbus_client_get_parameters(registers, 0, 18);
	for (int i = 0; i < 3; i++) {
		rawMin[i] = registers[6 * i + 0];
		rawMax[i] = registers[6 * i + 1];
		realMin[i] = uint16_to_float(registers[6 * i + 3], registers[6 * i + 2]);
		realMax[i] = uint16_to_float(registers[6 * i + 5], registers[6 * i + 4]);
	}
	return status;
}

bool Tools_Communications_Devices_WellerToJBC_setParameters(const char port[], const int address, const float A, const float B, const float qw, const float rv, const float qz, const float s, const float psi, const float alpha, const float antiwindup, const float umin, const float umax, const float zmin, const float zmax, const float deltaumin, const float deltaumax) {
	modbus_client_set_RTU_address(address);
	modbus_set_serial_port(port);
	uint16_t registers[30];
	float_to_uint16(A, &registers[1], &registers[0]);
	float_to_uint16(B, &registers[3], &registers[2]);
	float_to_uint16(qw, &registers[5], &registers[4]);
	float_to_uint16(rv, &registers[7], &registers[6]);
	float_to_uint16(qz, &registers[9], &registers[8]);
	float_to_uint16(s, &registers[11], &registers[10]);
	float_to_uint16(psi, &registers[13], &registers[12]);
	float_to_uint16(alpha, &registers[15], &registers[14]);
	float_to_uint16(antiwindup, &registers[17], &registers[16]);
	float_to_uint16(umin, &registers[19], &registers[18]);
	float_to_uint16(umax, &registers[21], &registers[20]);
	float_to_uint16(zmin, &registers[23], &registers[22]);
	float_to_uint16(zmax, &registers[25], &registers[24]);
	float_to_uint16(deltaumin, &registers[27], &registers[26]);
	float_to_uint16(deltaumax, &registers[29], &registers[28]);
	const bool status = modbus_client_set_parameters(registers, 18, 30);
	return status;
}

bool Tools_Communications_Devices_WellerToJBC_getParameters(const char port[], const int address, float* A, float* B, float* qw, float* rv, float* qz, float* s, float* psi, float* alpha, float* antiwindup, float* umin, float* umax, float* zmin, float* zmax, float* deltaumin, float* deltaumax) {
	modbus_client_set_RTU_address(address);
	modbus_set_serial_port(port);
	uint16_t registers[30];
	const bool status = modbus_client_get_parameters(registers, 18, 30);
	*A = uint16_to_float(registers[1], registers[0]);
	*B = uint16_to_float(registers[3], registers[2]);
	*qw = uint16_to_float(registers[5], registers[4]);
	*rv = uint16_to_float(registers[7], registers[6]);
	*qz = uint16_to_float(registers[9], registers[8]);
	*s = uint16_to_float(registers[11], registers[10]);
	*psi = uint16_to_float(registers[13], registers[12]);
	*alpha = uint16_to_float(registers[15], registers[14]);
	*antiwindup = uint16_to_float(registers[17], registers[16]);
	*umin = uint16_to_float(registers[19], registers[18]);
	*umax = uint16_to_float(registers[21], registers[20]);
	*zmin = uint16_to_float(registers[23], registers[22]);
	*zmax = uint16_to_float(registers[25], registers[24]);
	*deltaumin = uint16_to_float(registers[27], registers[26]);
	*deltaumax = uint16_to_float(registers[29], registers[28]);
	return status;
}

bool Tools_Communications_Devices_WellerToJBC_setOperation(const char port[], const int address, const int operationIndex) {
	modbus_client_set_RTU_address(address);
	modbus_set_serial_port(port);
	uint16_t registers[1] = { operationIndex };
	return modbus_client_set_parameters(registers, 48, 1);
}

bool Tools_Communications_Devices_WellerToJBC_getOperation(const char port[], const int address, int* operationIndex) {
	modbus_client_set_RTU_address(address);
	modbus_set_serial_port(port);
	uint16_t registers[1];
	const bool status = modbus_client_get_parameters(registers, 48, 1);
	*operationIndex = registers[0];
	return status;
}