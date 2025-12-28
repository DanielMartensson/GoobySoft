#include "EasyCANopen.h"
#include "../../../Tools.h"

/* IDE in STM32 */
#define dSTANDARD_CAN_MSG_ID_2_0B 1
#define TX_CAN_MESSAGE_SIZE 14
#define RX_CAN_MESSAGE_SIZE 20

static std::vector<std::vector<std::string>> rows;
static int allowed_rows = 20;
static char _port[20] = {0};

void Tools_Software_Libraries_EasyCANopen_callbackFunctionSend(uint16_t COB_ID, uint8_t DLC, uint8_t data[]) {
	uint8_t dataTX[TX_CAN_MESSAGE_SIZE] = { 0 };
	dataTX[0] = dSTANDARD_CAN_MSG_ID_2_0B;
	dataTX[1] = 0;
	dataTX[2] = 0;
	dataTX[3] = (COB_ID >> 8) & 0xFF;
	dataTX[4] = COB_ID & 0xFF;
	dataTX[5] = DLC;
	for (uint8_t i = 0; i < DLC; i++) {
		dataTX[6 + i] = data[i];
	}
	Tools_Hardware_USB_write(_port, dataTX, 6 + DLC, 100);
}

void Tools_Software_Libraries_EasyCANopen_callbackFunctionRead(uint16_t* COB_ID, uint8_t data[], bool* is_new_data) {
	// Check available bytes
	int32_t availableBytes = Tools_Hardware_USB_availableBytes(_port);
	if(availableBytes <= 0){
		return;
	}

	// Read 
	std::vector<uint8_t> dataRX = std::vector<uint8_t>(availableBytes);
	int32_t received = Tools_Hardware_USB_read(_port, dataRX.data(), availableBytes, 100, false, true);
	if (received <= 0) {
		*is_new_data = false;
		return;
	}

	// Search
	int32_t startIndex;
	for(startIndex = 0; startIndex < received - RX_CAN_MESSAGE_SIZE; startIndex++){
		if(dataRX.at(0 + startIndex) == dSTANDARD_CAN_MSG_ID_2_0B && dataRX.at(14 + startIndex) == 'S' && dataRX.at(15 + startIndex) == 'T' && dataRX.at(16 + startIndex) == 'M' && dataRX.at(17 + startIndex) == '3' && dataRX.at(18 + startIndex) == '2' && dataRX.at(19 + startIndex) == '\0'){
			*is_new_data = true;
			break;
		}
	}

	if(*is_new_data){
		*COB_ID = (dataRX.at(1 + startIndex) << 24) | (dataRX.at(2 + startIndex) << 16) | (dataRX.at(3 + startIndex) << 8) | dataRX.at(4 + startIndex);
		uint8_t DLC = dataRX.at(5 + startIndex);
		for (uint8_t i = 0; i < DLC; i++) {
			data[i] = dataRX.at(i + 6 + startIndex);
		}

		/* The message was successfully read - Remove it then */
		Tools_Hardware_USB_eraseData(_port, startIndex, RX_CAN_MESSAGE_SIZE);
	}else{
		*is_new_data = false;
	}
}

void Tools_Software_Libraries_EasyCANopen_callbackFunctionTraffic(uint16_t ID, uint8_t DLC, uint8_t data[], bool is_TX) {
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

int* Tools_Software_Libraires_EasyCANopen_getCANTrafficAllowedRowsPtr() {
	return &allowed_rows;
}

void Tools_Software_Libraries_EasyCANopen_setPort(const char port[]){
	std::strcpy(_port, port);
}