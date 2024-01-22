#ifndef Parameters
#define Parameters
#include "../../Hardware/USB/Protocols/OpenSAEJ1939/OpenSAEJ1939.h"
#include "../../Software/Libraries/CControl/ccontrol.h"

#define MAX_PROTOCOLS 5					// How many protocols can be used, CDC, Modbus RTU, Modbus TCP etc..
#define MAX_DEVICES 10					// How many devices per protocol
#define MAX_ROWS 10						// How many rows per device inside configuration window
#define MAX_COLUMNS 10					// Max columns for each device inside configuration column
#define MAX_USB_PORTS 10				// Max USB ports that can be connected at the same time
#define MAX_C_STRING_LEN 30				// Max char* length for e.g port, device name etc.
#define MAX_C_STRING_EXTRA_LEN 1024		// Max char* length for function values
#define MAX_DATA_MEASUREMENT_PLOT 1024	// Max plot length for real time measuring

typedef enum {
	COLUMN_DEFINITION_PORT,				// Define a column as port	- Mandatory column
	COLUMN_DEFINITION_DISPLAY_NAME,		// Define a column as display name - Mandatory column
	COLUMN_DEFINITION_FUNCTION,			// Define a column as function - Mandatory column
	COLUMN_DEFINITION_MIN_VALUE_REAL,	// For control or measurement calibration
	COLUMN_DEFINITION_MIN_VALUE_RAW,	// For control or measurement calibration
	COLUMN_DEFINITION_MAX_VALUE_REAL,	// For measurement calibration
	COLUMN_DEFINITION_MAX_VALUE_RAW,	// For measurement calibration
	COLUMN_DEFINITION_ADDRESS			// For e.g CAN-bus or Modbus communication
}COLUMN_DEFINITION;

typedef enum {
	COLUMN_TYPE_UNKNOWN,				// Unknow column type, this is an error message
	COLUMN_TYPE_COMBO,					// For the function and port column
	COLUMN_TYPE_INT,					// For control or measurement calibration
	COLUMN_TYPE_FLOAT,					// For measurement calibration
	COLUMN_TYPE_STRING					// For display name column
}COLUMN_TYPE;

typedef enum {
	COLUMN_FUNCTION_HOLD_DATA,							// Regular field e.g Device Name column
	COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS,				// For e.g pre calibrated analog sensors but it's for CAN-bus/Modbus. 
	COLUMN_FUNCTION_INPUT_SENSOR_NO_CALIBRATION,		// For e.g digital inputs or analog sensors that don't requrire calibration
	COLUMN_FUNCTION_INPUT_SENSOR_ANALOG,				// For e.g analog sensors that need calibration
	COLUMN_FUNCTION_OUTPUT_ACTUATOR,					// For e.g PWM outputs, or outputs with fixed address e.g Auxiliary Valve Command inside SAE J1939
	COLUMN_FUNCTION_OUTPUT_ACTUATOR_ADDRESS				// For e.g PWM but it's for CAN-bus/Modbus
}COLUMN_FUNCTION;

struct TableColumnID {
	char columnName[MAX_C_STRING_LEN] = "";							// What is the name of the column
	COLUMN_DEFINITION columnDefinition = COLUMN_DEFINITION_PORT;	// What is the definition of the column
	COLUMN_TYPE columnType = COLUMN_TYPE_INT;						// What is the data type of the cell value
	COLUMN_FUNCTION columnFunction = COLUMN_FUNCTION_HOLD_DATA;		// What the column should do
};

struct TableColumn {
	// Contains the definition, type and function
	TableColumnID tableColumnID;

	// Values of the cell, depending on COLUMN_TYPE
	char cellValueString[MAX_C_STRING_LEN] = "";
	int cellValueInt = 0;
	float cellValueFloat = 0;

	// For storing combo stuffs, depending on COLUMN_DEFINITION
	char functionValues[MAX_C_STRING_EXTRA_LEN] = "";
	int functionValueIndex = 0;
};

struct TableRow {
	// Columns of row
	TableColumn tableColumns[MAX_COLUMNS];
	bool isHeader = false;
	int tableColumnCount = 0;

	// Measurement data
	float xData[MAX_DATA_MEASUREMENT_PLOT] = { 0.0f };
	float yData[MAX_DATA_MEASUREMENT_PLOT] = { 0.0f };

	// For slider
	int sliderValue = 0;

	// IO
	float (*getInput)(const char[], int, int);
	bool (*setOutput)(const char[], int, int, int);

	// For the function
	COLUMN_FUNCTION (*getColumnFunction)(int);
};

struct Device {
	TableRow tableRows[MAX_ROWS];
	TableRow tableRowSelected;
	int selectedRowIndex = 0;
	int tableRowCount = 0;
	char deviceName[MAX_C_STRING_LEN] = "";
};

struct Protocol {
	Device devices[MAX_DEVICES];
	int deviceCount = 0;
	bool isProtocolUsed = false;
	char protocolName[MAX_C_STRING_LEN] = "";
};

struct USBSettings {
	unsigned int baudrate = 9600;
	unsigned int dataBit = 8;
	int parityIndex = 0;
	int stopBitIndex = 0;
	int controlFlowIndex = 0;
	int protocolIndex = 0;
	char port[MAX_C_STRING_LEN] = "";
};

struct ModbusSettings {
	int deviceAddress = 1;
};

struct FileSettings {
	char filePathName[MAX_C_STRING_EXTRA_LEN] = "";
	char folderPath[MAX_C_STRING_EXTRA_LEN] = "";
	char fileName[MAX_C_STRING_LEN] = "";
};

struct DatabaseSettings {
	char host[MAX_C_STRING_LEN] = "127.0.0.1";
	char schema[MAX_C_STRING_LEN] = "schema";
	int addDevice = 33060;
	char username[MAX_C_STRING_LEN] = "myUser";
	char password[MAX_C_STRING_LEN] = "myPassword";
};

typedef struct {
	USBSettings usbSettings[MAX_USB_PORTS];
	ModbusSettings modbusSettings[MAX_USB_PORTS];
	DatabaseSettings databaseSettings;
	FileSettings fileSettings;
	Protocol protocols[MAX_PROTOCOLS];
	MODEL_SETTINGS model_settings;
	MODEL models;
}ParameterHolder;

void Tools_Hardware_ParameterStore_loadParameters();
void Tools_Hardware_ParameterStore_saveParameters();
void* Tools_Hardware_ParameterStore_readCellvalueAtColumnDefinition(TableColumn* tableColumns, int tableColumnCount, COLUMN_DEFINITION columnDefinition, COLUMN_TYPE* columnType);
COLUMN_FUNCTION Tools_Hardware_ParameterStore_readColumnFunctionAtColumnDefinition(TableColumn* tableColumns, int tableColumnCount, COLUMN_DEFINITION columnDefinition);
ParameterHolder* Tools_Hardware_ParameterStore_getParameterHolder();
J1939* Tools_Hardware_ParameterStore_getJ1939();

#endif // !Parameters