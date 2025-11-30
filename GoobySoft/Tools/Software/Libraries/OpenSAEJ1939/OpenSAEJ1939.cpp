#include "OpenSAEJ1939.h"
#include "../../../Tools.h"

/* IDE in STM32 */
#define dEXTENDED_CAN_MSG_ID_2_0B 2
#define dataTXSize 14
#define dataRXsize 40
#define CANmessageSize 20

static std::vector<std::vector<std::string>> rows;
static int allowed_rows = 20;
static char _port[20] = {0};

void Tools_Software_Libraries_OpenSAEJ1939_callbackFunctionSend(uint32_t ID, uint8_t DLC, uint8_t data[]) {
	uint8_t dataTX[dataTXSize] = { 0 };
	dataTX[0] = dEXTENDED_CAN_MSG_ID_2_0B;
	dataTX[1] = ID >> 24;
	dataTX[2] = ID >> 16;
	dataTX[3] = ID >> 8;
	dataTX[4] = ID;
	dataTX[5] = DLC;
	for (uint8_t i = 0; i < DLC; i++) {
		dataTX[6 + i] = data[i];
	}
	Tools_Hardware_USB_write(_port, dataTX, 6 + DLC, 100);
}

void Tools_Software_Libraries_OpenSAEJ1939_callbackFunctionRead(uint32_t* ID, uint8_t data[], bool* is_new_data) {
	uint8_t dataRX[dataRXsize] = { 0 }; 
	int32_t received = Tools_Hardware_USB_read(_port, dataRX, dataRXsize, 100);
	if (received <= 0) {
		*is_new_data = false;
		return;
	}

	uint8_t startIndex;
	for(startIndex = 0; startIndex < dataRXsize - CANmessageSize; startIndex++){
		if(dataRX[0 + startIndex] == dEXTENDED_CAN_MSG_ID_2_0B && dataRX[14 + startIndex] == 'S' && dataRX[15 + startIndex] == 'T' && dataRX[16 + startIndex] == 'M' && dataRX[17 + startIndex] == '2' && dataRX[18 + startIndex] == '3' && dataRX[19 + startIndex] == '\0'){
			*is_new_data = true;
			break;
		}
	}

	if(*is_new_data){
		*ID = (dataRX[3 + startIndex] << 8) | dataRX[4 + startIndex];
		uint8_t DLC = dataRX[5 + startIndex];
		for (int i = 0; i < DLC; i++) {
			data[i] = dataRX[i + 6 + startIndex];
		}

		/* The message was successfully read - Remove it then */
		Tools_Hardware_USB_eraseData(_port, startIndex, CANmessageSize);
	}else{
		*is_new_data = false;
	}
}

void Tools_Software_Libraries_OpenSAEJ1939_callbackFunctionTraffic(uint32_t ID, uint8_t DLC, uint8_t data[], bool is_TX) {
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

void Tools_Software_Libraries_OpenSAEJ1939_callbackDelay(uint8_t delay) {

}

std::vector<std::vector<std::string>>& Tools_Software_Libraires_OpenSAEJ1939_getCANTrafficRows() {
	return rows;
}

int* Tools_Software_Libraires_OpenSAEJ1939_getCANTrafficAllowedRowsPtr() {
	return &allowed_rows;
}

void Tools_Software_Libraries_OpenSAEJ1939_setPort(const char port[]){
	std::strcpy(_port, port);
}