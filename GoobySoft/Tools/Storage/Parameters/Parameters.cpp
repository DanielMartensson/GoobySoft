#include "Parameters.h"
#include <fstream>
#include <cstring>

#define FILE_NAME "parameters.dat"
#define SIZE_PARAMETER_HOLDER sizeof(ParameterHolder)

static ParameterHolder parameters;

void Tools_Hardware_ParameterStore_loadParameters() {
	// Turn the struct to an array of data
	uint8_t data[SIZE_PARAMETER_HOLDER];
	std::memset(data, 0, SIZE_PARAMETER_HOLDER);

	// Read from file
	std::ifstream file;
	file.open(FILE_NAME);
	if (file.is_open()) {
		file.read((char*)data, SIZE_PARAMETER_HOLDER);
		std::memcpy(&parameters, data, SIZE_PARAMETER_HOLDER);
	}
}

void Tools_Hardware_ParameterStore_saveParameters() {
	// Turn the struct to an array of data
	uint8_t data[SIZE_PARAMETER_HOLDER];
	std::memcpy(data,&parameters, SIZE_PARAMETER_HOLDER);

	// Write to file
	std::ofstream file;
	file.open(FILE_NAME);
	if (file.is_open()) {
		file.write((char*)data, SIZE_PARAMETER_HOLDER);
		file.close();
	}
}

ParameterHolder* Tools_Hardware_ParameterStore_getParameterHolder() {
	return &parameters;
}