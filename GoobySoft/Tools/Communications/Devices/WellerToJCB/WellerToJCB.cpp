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
	IO_READ_ITERATION_TIME_MS,
	IO_READ_INPUT_OPTIMIZED
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
	functionNames += "Read optimized input signal";
	functionNames += '\0';
	return functionNames;
}
std::vector<TableColumnID> Tools_Communications_Devices_WellerToJBC_getTableColumnIDs() {
	std::vector<TableColumnID> tableColumnIDs;
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Port", COLUMN_DEFINITION_PORT)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Function", COLUMN_DEFINITION_FUNCTION)); // Mandatory field
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Modbus address", COLUMN_DEFINITION_ADDRESS));
	tableColumnIDs.emplace_back(Tools_Communications_Devices_createTableIDs("Display name", COLUMN_DEFINITION_DISPLAY_NAME)); // Mandatory field
	return tableColumnIDs;
}
float Tools_Communications_Devices_WellerToJBC_getInput(const char port[], int functionValueIndex, int address) {
	/* These must follow the same linear pattern as getFunctionValues() */
	modbus_client_set_RTU_address(address);
	modbus_set_serial_port(port);
	uint16_t registers[100];
	uint8_t digitals[100];
	switch (functionValueIndex) {
	case IO_READ_CURRENT_RAW: {
		static float value = 0;
		if (modbus_client_get_analog_inputs(registers, 2, 1)) {
			value = (float)registers[0];
		}
		return value;
	}
	case IO_READ_CURRENT_REAL: {
		static float value = 0;
		if (modbus_client_get_analog_inputs(registers, 7, 2)) {
			value = uint16_to_float(registers[1], registers[0]);
		}
		return value;
	}
	case IO_READ_TEMPERATURE_RAW: {
		static float value = 0;
		if (modbus_client_get_analog_inputs(registers, 0, 1)) {
			value = (float)registers[0];
		}
		return value;
	}
	case IO_READ_TEMPERATURE_REAL: {
		static float value = 0;
		if (modbus_client_get_analog_inputs(registers, 3, 2)) {
			value = uint16_to_float(registers[1], registers[0]);
		}
		return value;
	}
	case IO_READ_SETPOINT_RAW: {
		static float value = 0;
		if (modbus_client_get_analog_inputs(registers, 1, 1)) {
			value = (float)registers[0];
		}
		return value;
	}
	case IO_READ_SETPOINT_REAL: {
		static float value = 0;
		if (modbus_client_get_analog_inputs(registers, 5, 2)) {
			value = uint16_to_float(registers[1], registers[0]);
		}
		return value;
	}
	case IO_READ_STATE_XHAT: {
		static float value = 0;
		if (modbus_client_get_analog_inputs(registers, 9, 2)) {
			value = uint16_to_float(registers[1], registers[0]);
		}
		return value;
	}
	case IO_READ_LED_GREEN: {
		static float value = 0;
		if (modbus_client_get_digital_outputs(digitals, 1, 1)) {
			value = (float)digitals[0];
		}
		return value;
	}
	case IO_READ_LEACKAGE_ACTIVE: {
		static float value = 0;
		if (modbus_client_get_digital_outputs(digitals, 0, 1)) {
			value = (float)digitals[0];
		}
		return value;
	}
	case IO_READ_SLEEP: {
		static float value = 0;
		if (modbus_client_get_digital_inputs(digitals, 0, 1)) {
			value = (float)digitals[0];
		}
		return value;
	}
	case IO_READ_ITERATION_TIME_MS: {
		static float value = 0;
		if (modbus_client_get_analog_inputs(registers, 11, 1)) {
			value = registers[0];
		}
		return value;
	}
	case IO_READ_INPUT_OPTIMIZED: {
		static float value = 0;
		if (modbus_client_get_analog_inputs(registers, 12, 2)) {
			value = uint16_to_float(registers[1], registers[0]);
		}
		return value;
	}
	default:
		return -1.0f;
	}
}

bool Tools_Communications_Devices_WellerToJBC_setOutput(const char port[], int functionValueIndex, int address, int value) {
	switch (functionValueIndex) {
	default:
		return false; // There are no set-outputs for WellerToJCB
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
	case IO_READ_INPUT_OPTIMIZED:
		return COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS_NO_CALIBRATION;
	default:
		return COLUMN_FUNCTION_HOLD_DATA; // Just hold data
	}
}

bool Tools_Communications_Devices_WellerToJBC_setAnalogInputCalibration(const char port[], const int address, const int rawMin[], const int rawMax[], const float realMin[], const float realMax[]) {
	modbus_client_set_RTU_address(address);
	modbus_set_serial_port(port);
	uint16_t registers[100];
	registers[0] = rawMin[0];
	registers[1] = rawMax[0];
	float_to_uint16(realMin[0], &registers[3], &registers[2]);
	float_to_uint16(realMax[0], &registers[5], &registers[4]);
	registers[6] = rawMin[1];
	registers[7] = rawMax[1];
	float_to_uint16(realMin[1], &registers[9], &registers[8]);
	float_to_uint16(realMax[1], &registers[11], &registers[10]);
	registers[12] = rawMin[2];
	registers[13] = rawMax[2];
	float_to_uint16(realMin[2], &registers[15], &registers[14]);
	float_to_uint16(realMax[2], &registers[17], &registers[16]);
	const bool status = modbus_client_set_parameters(registers, 0, 18);
	Tools_Hardware_USB_flush(port);
	return status;
}

bool Tools_Communications_Devices_WellerToJBC_getAnalogInputCalibration(const char port[], const int address, int rawMin[], int rawMax[], float realMin[], float realMax[]) {
	modbus_client_set_RTU_address(address);
	modbus_set_serial_port(port);
	uint16_t registers[100];
	const bool status = modbus_client_get_parameters(registers, 0, 18);
	Tools_Hardware_USB_flush(port);
	rawMin[0] = registers[0];
	rawMax[0] = registers[1];
	realMin[0] = uint16_to_float(registers[3], registers[2]);
	realMax[0] = uint16_to_float(registers[5], registers[4]);
	rawMin[1] = registers[6];
	rawMax[1] = registers[7];
	realMin[1] = uint16_to_float(registers[9], registers[8]);
	realMax[1] = uint16_to_float(registers[11], registers[10]);
	rawMin[2] = registers[12];
	rawMax[2] = registers[13];
	realMin[2] = uint16_to_float(registers[15], registers[14]);
	realMax[2] = uint16_to_float(registers[17], registers[16]);
	return status;
}

bool Tools_Communications_Devices_WellerToJBC_setParameters(const char port[], const int address, const float A, const float B, const float qw, const float rv, const float qz, const float s, const float psi, const float alpha, const float antiwindup, const float umin, const float umax, const float steadyStateModelError, const float zmax, const float deltaumin, const float deltaumax) {
	modbus_client_set_RTU_address(address);
	modbus_set_serial_port(port);
	uint16_t registers[100];
	float_to_uint16(A, &registers[1], &registers[0]);
	float_to_uint16(B, &registers[3], &registers[2]);
	float_to_uint16(qw, &registers[5], &registers[4]);
	float_to_uint16(rv, &registers[7], &registers[6]);
	float_to_uint16(qz, &registers[9], &registers[8]);
	float_to_uint16(s, &registers[11], &registers[10]);
	float_to_uint16(psi, &registers[13], &registers[12]);
	float_to_uint16(alpha, &registers[15], &registers[14]);
	const bool status1 = modbus_client_set_parameters(registers, 18, 16);
	Tools_Hardware_USB_flush(port);
	float_to_uint16(antiwindup, &registers[1], &registers[0]);
	float_to_uint16(umin, &registers[3], &registers[2]);
	float_to_uint16(umax, &registers[5], &registers[4]);
	float_to_uint16(steadyStateModelError, &registers[7], &registers[6]);
	float_to_uint16(zmax, &registers[9], &registers[8]);
	float_to_uint16(deltaumin, &registers[11], &registers[10]);
	float_to_uint16(deltaumax, &registers[13], &registers[12]);
	const bool status2 = modbus_client_set_parameters(registers, 34, 14);
	Tools_Hardware_USB_flush(port);
	return status1 && status2;
}

bool Tools_Communications_Devices_WellerToJBC_getParameters(const char port[], const int address, float* A, float* B, float* qw, float* rv, float* qz, float* s, float* psi, float* alpha, float* antiwindup, float* umin, float* umax, float* steadyStateModelError, float* zmax, float* deltaumin, float* deltaumax) {
	modbus_client_set_RTU_address(address);
	modbus_set_serial_port(port);
	uint16_t registers[100];
	const bool status1 = modbus_client_get_parameters(registers, 18, 16);
	Tools_Hardware_USB_flush(port);
	*A = uint16_to_float(registers[1], registers[0]);
	*B = uint16_to_float(registers[3], registers[2]);
	*qw = uint16_to_float(registers[5], registers[4]);
	*rv = uint16_to_float(registers[7], registers[6]);
	*qz = uint16_to_float(registers[9], registers[8]);
	*s = uint16_to_float(registers[11], registers[10]);
	*psi = uint16_to_float(registers[13], registers[12]);
	*alpha = uint16_to_float(registers[15], registers[14]);
	const bool status2 = modbus_client_get_parameters(registers, 34, 14);
	Tools_Hardware_USB_flush(port);
	*antiwindup = uint16_to_float(registers[1], registers[0]);
	*umin = uint16_to_float(registers[3], registers[2]);
	*umax = uint16_to_float(registers[5], registers[4]);
	*steadyStateModelError = uint16_to_float(registers[7], registers[6]);
	*zmax = uint16_to_float(registers[9], registers[8]);
	*deltaumin = uint16_to_float(registers[11], registers[10]);
	*deltaumax = uint16_to_float(registers[13], registers[12]);
	return status1 && status2;
}

bool Tools_Communications_Devices_WellerToJBC_setOperation(const char port[], const int address, const int operationIndex) {
	modbus_client_set_RTU_address(address);
	modbus_set_serial_port(port);
	uint16_t registers[10] = { operationIndex };
	const bool status = modbus_client_set_parameters(registers, 48, 1);
	Tools_Hardware_USB_flush(port);
	return status;
}

bool Tools_Communications_Devices_WellerToJBC_getOperation(const char port[], const int address, int* operationIndex) {
	modbus_client_set_RTU_address(address);
	modbus_set_serial_port(port);
	uint16_t registers[100];
	const bool status = modbus_client_get_parameters(registers, 48, 1);
	Tools_Hardware_USB_flush(port);
	*operationIndex = registers[0];
	return status;
}