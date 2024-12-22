#include "Devices.h"
#include "../../Tools.h"

void createDeviceTool(struct Device* devices, const char deviceName[], std::string (*functionValues)(void), std::vector<TableColumnID> (*tableColumnIDs)(void), float (*getInput)(const char[], int, int), bool (*setOutput)(const char[], int, int, int), COLUMN_FUNCTION(*getColumnFunction)(int)) {
	std::strcpy(devices->deviceName, deviceName);															
	// Iterate the rows
	TableRow* tableRows = devices->tableRows;
	for (int i = 0; i < MAX_ROWS; i++) {
		// Check if it's the first row. The first row is the header row
		tableRows[i].isHeader = i == 0;	

		// Set column count
		tableRows[i].tableColumnCount = tableColumnIDs().size();

		// Set the input callback
		tableRows[i].getInput = getInput;

		// Set the output callback
		tableRows[i].setOutput = setOutput;

		// Set the column function callback 
		tableRows[i].getColumnFunction = getColumnFunction;
		
		// Iterate the columns
		TableColumn* tableColumns = tableRows[i].tableColumns;
		int tableColumnCount = tableRows[i].tableColumnCount;
		for (int j = 0; j < tableColumnCount; j++) {
			// Get table column ID
			TableColumnID tableColumnID = tableColumnIDs().at(j);
			
			// Set column names
			std::strcpy(tableColumns[j].tableColumnID.columnName, tableColumnID.columnName);
			
			// Set the column definition
			tableColumns[j].tableColumnID.columnDefinition = tableColumnID.columnDefinition;

			// Set the column type
			tableColumns[j].tableColumnID.columnType = tableColumnID.columnType;

			// Set the column function values
			if(tableColumns[j].tableColumnID.columnDefinition == COLUMN_DEFINITION_FUNCTION){
				std::memcpy(tableColumns[j].functionValues, functionValues().c_str(), sizeof(tableColumns[j].functionValues)); // std::strcpy will stop at first '\0', std::memcpy will not
			}
		}
	}
}

void createProtocolTool(struct Protocol* protocols, const std::string protocolName, int deviceCount) {
	std::strcpy(protocols->protocolName, protocolName.c_str());
	protocols->deviceCount = deviceCount;
	protocols->isProtocolUsed = true;
}

TableColumnID Tools_Communications_Devices_createTableIDs(const char columName[], COLUMN_DEFINITION columnDefinition) {
	// Create the table ID
	TableColumnID tableColumnID;

	// Set column name
	std::strcpy(tableColumnID.columnName, columName);

	// Set definition
	tableColumnID.columnDefinition = columnDefinition;

	// Set column type
	switch (columnDefinition) {
	case COLUMN_DEFINITION_PORT:
		tableColumnID.columnType = COLUMN_TYPE_COMBO;
		break;
	case COLUMN_DEFINITION_DISPLAY_NAME:
		tableColumnID.columnType = COLUMN_TYPE_STRING;
		break;
	case COLUMN_DEFINITION_FUNCTION:
		tableColumnID.columnType = COLUMN_TYPE_COMBO;
		break;
	case COLUMN_DEFINITION_MIN_VALUE_REAL:
		tableColumnID.columnType = COLUMN_TYPE_FLOAT;
		break;
	case COLUMN_DEFINITION_MIN_VALUE_RAW:
		tableColumnID.columnType = COLUMN_TYPE_INT;
		break;
	case COLUMN_DEFINITION_MAX_VALUE_REAL:
		tableColumnID.columnType = COLUMN_TYPE_FLOAT;
		break;
	case COLUMN_DEFINITION_MAX_VALUE_RAW:
		tableColumnID.columnType = COLUMN_TYPE_INT;
		break;
	case COLUMN_DEFINITION_ADDRESS:
		tableColumnID.columnType = COLUMN_TYPE_INT;
		break;
	}
	
	// Return the column ID
	return tableColumnID;
}

void Tools_Communications_Devices_createDevices() {
	// Get the parameter holder
	Protocol* protocols = Tools_Hardware_ParameterStore_getParameterHolder()->protocols;

	// Reset all
	for (int i = 0; i < MAX_PROTOCOLS; i++) {
		protocols[i].isProtocolUsed = false;
	}

	// Create devices for protocols 
	createProtocolTool(&protocols[0], PROTOCOL_STRING[PROTOCOL_ENUM_MODBUS_RTU], 3); // Modbus RTU, 3 device
	createProtocolTool(&protocols[1], PROTOCOL_STRING[PROTOCOL_ENUM_CDC], 1); // CDC, 1 device
	// Add new protocol here...

	// Create modbus RTU devices
	createDeviceTool(&protocols[0].devices[0], "ADL400", Tools_Communications_Devices_ADL400_getFunctionValues, Tools_Communications_Devices_ADL400_getTableColumnIDs, Tools_Communications_Devices_ADL400_getInput, Tools_Communications_Devices_ADL400_setOutput, Tools_Communications_Devices_ADL400_getColumnFunction);
	createDeviceTool(&protocols[0].devices[1], "Uponor X-141", Tools_Communications_Devices_Uponor_X_148_getFunctionValues, Tools_Communications_Devices_Uponor_X_148_getTableColumnIDs, Tools_Communications_Devices_Uponor_X_148_getInput, Tools_Communications_Devices_Uponor_X_148_setOutput, Tools_Communications_Devices_Uponor_X_148_getColumnFunction);
	createDeviceTool(&protocols[0].devices[2], "WellerToJBC", Tools_Communications_Devices_WellerToJBC_getFunctionValues, Tools_Communications_Devices_WellerToJBC_getTableColumnIDs, Tools_Communications_Devices_WellerToJBC_getInput, Tools_Communications_Devices_WellerToJBC_setOutput, Tools_Communications_Devices_WellerToJBC_getColumnFunction);
	// Add new device here...

	// Create CDC devices
	createDeviceTool(&protocols[1].devices[0], "STM32 PLC", Tools_Communications_Devices_STM32PLC_getFunctionValues, Tools_Communications_Devices_STM32PLC_getTableColumnIDs, Tools_Communications_Devices_STM32PLC_getInput, Tools_Communications_Devices_STM32PLC_setOutput, Tools_Communications_Devices_STM32PLC_getColumnFunction);
	// Add new device here...
}

void Tools_Communications_Devices_updatePorts() {
	// Get the parameter holder
	Protocol* protocols = Tools_Hardware_ParameterStore_getParameterHolder()->protocols;

	// Update ports of the rows
	for (int i = 0; i < MAX_PROTOCOLS; i++) {
		// Check if the protocols is available
		if (protocols[i].isProtocolUsed) {
			// Iterate the devices
			Device* devices = protocols[i].devices;
			int deviceCount = protocols[i].deviceCount;
			for (int j = 0; j < deviceCount; j++) {
				// Iterate the rows
				TableRow* tableRows = devices[j].tableRows;
				int tableRowCount = devices[j].tableRowCount;
				for (int k = 0; k <= tableRowCount; k++) {
					// Iterate the columns
					TableColumn* tableColumns = tableRows[k].tableColumns;
					int tableColumnCount = tableRows[k].tableColumnCount;
					for (int l = 0; l < tableColumnCount; l++) {
						// Check if the column is port column
						if (tableColumns[l].tableColumnID.columnDefinition == COLUMN_DEFINITION_PORT) {
							bool isUSB = (std::strcmp(PROTOCOL_STRING[PROTOCOL_ENUM_MODBUS_RTU].c_str(), protocols[i].protocolName) == 0) ||
								         (std::strcmp(PROTOCOL_STRING[PROTOCOL_ENUM_CDC].c_str(), protocols[i].protocolName) == 0);
							if (isUSB) {
								// Copy over the USB ports
								std::strcpy(tableColumns[l].functionValues, Tools_Hardware_USB_getConnectedPorts().c_str());
							}
							bool isTCP = (std::strcmp(PROTOCOL_STRING[PROTOCOL_ENUM_MODBUS_TCP].c_str(), protocols[i].protocolName) == 0);
							if (isTCP) {
								// Copy over the connected IP addresses
								std::strcpy(tableColumns[l].functionValues, Tools_Hardware_Network_getConnectedIpAddresses().c_str());
							}
						}
					}
				}
			}
		}
	}
}

void Tools_Communications_Devices_clear() {
	// Get the parameter holder
	Protocol* protocols = Tools_Hardware_ParameterStore_getParameterHolder()->protocols;

	// Clear the data arrays of the rows
	for (int i = 0; i < MAX_PROTOCOLS; i++) {
		// Check if the protocols is available
		if (protocols[i].isProtocolUsed) {
			// Iterate the devices
			Device* devices = protocols[i].devices;
			int deviceCount = protocols[i].deviceCount;
			for (int j = 0; j < deviceCount; j++) {
				// Iterate the rows
				TableRow* tableRows = devices[j].tableRows;
				int tableRowCount = devices[j].tableRowCount;
				for (int k = 0; k < tableRowCount; k++) {
					// Clear data
					std::fill_n(tableRows[k].xData, MAX_DATA_MEASUREMENT_PLOT, 0.0f);
					std::fill_n(tableRows[k].yData, MAX_DATA_MEASUREMENT_PLOT, 0.0f);
				}
			}
		}
	}
}