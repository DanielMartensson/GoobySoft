#include "EasyCANopen.h"
#include "../../../Tools.h"

/* IDE in STM32 */
#define CAN_ID_STD                  (0x00000000U)  /*!< Standard Id */
#define CAN_ID_EXT                  (0x00000004U)  /*!< Extended Id */

static std::vector<std::vector<std::string>> rows;
static int allowed_rows = 10;

void Tools_Software_Libraries_EasyCANopen_callbackFunctionSend(uint32_t ID, uint8_t DLC, uint8_t data[]) {
	// Load the data. 7 + largest value of DLC = 15 
	uint8_t dataTX[15] = { 0 };
	dataTX[0] = STM32PLC_WRITE_SET_CAN_BUS_MESSAGE_TYPE;
	dataTX[1] = CAN_ID_EXT;
	dataTX[2] = ID >> 24;
	dataTX[3] = ID >> 16;
	dataTX[4] = ID >> 8;
	dataTX[5] = ID;
	dataTX[6] = DLC;
	for (uint8_t i = 0; i < DLC; i++) {
		dataTX[7 + i] = data[i];
	}

	// Get the port
	const char* port = Tools_Communications_Devices_STM32PLC_getAddressPort();

	Tools_Hardware_USB_write(port, dataTX, sizeof(dataTX), 1000);
}

void Tools_Software_Libraries_EasyCANopen_callbackFunctionRead(uint32_t* ID, uint8_t data[], bool* is_new_data) {
	// Get the port
	const char* port = Tools_Communications_Devices_STM32PLC_getAddressPort();

	// Ask for CAN message
	uint8_t CAN[30] = { STM32PLC_SEND_BACK_CAN_MESSAGE_TYPE };
	Tools_Hardware_USB_write(port, CAN, 1, 0);
	int32_t received = Tools_Hardware_USB_read(port, CAN, 30, 1000);
	if (received <= 0) {
		*is_new_data = false;
		return;
	}

	// Check if the CAN message back has extended ID
	if (data[0] == CAN_ID_STD) {
		*is_new_data = false;
	}
	else {
		*ID = (CAN[1] << 24) | (CAN[2] << 16) | (CAN[3] << 8) | CAN[4];
		uint8_t DLC = CAN[5];
		for (int i = 0; i < DLC; i++) {
			data[i] = CAN[6 + i];
		}
		*is_new_data = true;
	}
}

void Tools_Software_Libraries_EasyCANopen_callbackFunctionTraffic(uint32_t ID, uint8_t DLC, uint8_t data[], bool is_TX) {
	// Add header 
	if (rows.size() == 0) {
		std::vector<std::string> header = { "TX/RX", "ID", "DLC", "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7" };
		rows.emplace_back(header);
	}

	// Create new row
	char text[20];
	std::vector<std::string> row;
	row.emplace_back(is_TX ? "TX" : "RX");
	sprintf(text, "0x%X", ID);
	row.emplace_back(text);
	sprintf(text, "0x%X", DLC);
	row.emplace_back(text);
	for (int i = 0; i < 8; i++) {
		if (i < DLC) {
			sprintf(text, "0x%X", data[i]);
			row.emplace_back(text);
		}
		else {
			row.emplace_back("0x0");
		}
	}

	// Add the new row 
	rows.emplace_back(row);

	// Check if it's to much rows
	size_t total_rows = rows.size() - 1; // Don't count with the header
	if (total_rows > allowed_rows) {
		rows.erase(rows.begin() + 1, rows.begin() + 1 + total_rows - allowed_rows);
	}
}

void Tools_Software_Libraries_EasyCANopen_callbackDelay(uint8_t delay) {
    
}

std::vector<std::vector<std::string>>& Tools_Software_Libraires_EasyCANopen_getCANTrafficRows() {
	return rows;
}

int* Tools_Software_Libraires_EasyCANopen_getCANTrafficAllowedRows() {
	return &allowed_rows;
}