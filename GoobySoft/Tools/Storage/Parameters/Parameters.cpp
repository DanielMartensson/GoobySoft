#include "Parameters.h"
#include <fstream>
#include <cstring>

#define FILE_NAME "parameters.dat"
#define SIZE_PARAMETER_HOLDER sizeof(ParameterHolder)

static ParameterHolder parameters;
static J1939 j1939;

void Tools_Hardware_ParameterStore_loadParameters() {
	// Allocate memory on the heap
	std::unique_ptr<uint8_t[]> data(new uint8_t[SIZE_PARAMETER_HOLDER]);
	std::memset(data.get(), 0, SIZE_PARAMETER_HOLDER);

	// Read from file
	std::ifstream file(FILE_NAME, std::ios::binary);
	if (file.is_open()) {
		file.read(reinterpret_cast<char*>(data.get()), SIZE_PARAMETER_HOLDER);
		std::memcpy(&parameters, data.get(), SIZE_PARAMETER_HOLDER);
		file.close();
	}

	// Load J1939 struct
	Open_SAE_J1939_Startup_ECU(&j1939);
}

void Tools_Hardware_ParameterStore_saveParameters() {
	// Allocate memory on the heap
	std::unique_ptr<uint8_t[]> data(new uint8_t[SIZE_PARAMETER_HOLDER]);

	// Copy the struct to the array
	std::memcpy(data.get(), &parameters, SIZE_PARAMETER_HOLDER);

	// Write to file
	std::ofstream file(FILE_NAME, std::ios::binary);
	if (file.is_open()) {
		file.write(reinterpret_cast<char*>(data.get()), SIZE_PARAMETER_HOLDER);
		file.close();
	}

	// Save J1939 struct
	Open_SAE_J1939_Closedown_ECU(&j1939);
}

void* Tools_Hardware_ParameterStore_readCellvalueAtColumnDefinition(TableColumn* tableColumns, int tableColumnCount, COLUMN_DEFINITION columnDefinition, COLUMN_TYPE* columnType) {
	for (int m = 0; m < tableColumnCount; m++) {
		if (tableColumns[m].tableColumnID.columnDefinition == columnDefinition) {
			*columnType = tableColumns[m].tableColumnID.columnType;
			switch (tableColumns[m].tableColumnID.columnType) {
			case COLUMN_TYPE_FLOAT:
				return &tableColumns[m].cellValueFloat;
			case COLUMN_TYPE_INT:
				return &tableColumns[m].cellValueInt;
			case COLUMN_TYPE_STRING:
				return &tableColumns[m].cellValueString;
			case COLUMN_TYPE_COMBO:
				return &tableColumns[m].cellValueString;
			}
		}
	}
	*columnType = COLUMN_TYPE_UNKNOWN;
	return nullptr;
}

COLUMN_FUNCTION Tools_Hardware_ParameterStore_readColumnFunctionAtColumnDefinition(TableColumn* tableColumns, int tableColumnCount, COLUMN_DEFINITION columnDefinition) {
	for (int m = 0; m < tableColumnCount; m++) {
		if (tableColumns[m].tableColumnID.columnDefinition == columnDefinition) {
			return tableColumns[m].tableColumnID.columnFunction;
		}
	}
	return COLUMN_FUNCTION_HOLD_DATA;
}

ParameterHolder* Tools_Hardware_ParameterStore_getParameterHolder() {
	return &parameters;
}
J1939* Tools_Hardware_ParameterStore_getJ1939() {
	return &j1939;
}