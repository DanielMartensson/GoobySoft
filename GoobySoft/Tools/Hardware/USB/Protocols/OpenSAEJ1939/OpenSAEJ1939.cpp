#include "OpenSAEJ1939.h"
#include "../../../../Tools.h"

/* IDE in STM32 */
#define CAN_ID_STD                  (0x00000000U)  /*!< Standard Id */
#define CAN_ID_EXT                  (0x00000004U)  /*!< Extended Id */

void Tools_Hardware_USB_Protocol_OpenSAEJ1939_callbackFunctionSend(uint32_t ID, uint8_t DLC, uint8_t data[]) {
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
	const char* port = Tools_Communications_Devices_STM32PLC_getPortOpenSAEJ1939();

	Tools_Hardware_USB_Protocols_CDC_startTransieveProcesss(port, 1000, dataTX, sizeof(dataTX));
}

void Tools_Hardware_USB_Protocol_OpenSAEJ1939_callbackFunctionRead(uint32_t* ID, uint8_t data[], bool* is_new_data) {
	// Get the port
	const char* port = Tools_Communications_Devices_STM32PLC_getPortOpenSAEJ1939();
	
	// Ask for CAN message
	uint8_t dataTX[1] = { STM32PLC_SEND_BACK_CAN_MESSAGE_TYPE };
	std::vector<uint8_t> dataRX = Tools_Hardware_USB_Protocols_CDC_startTransieveProcesss(port, 1000, dataTX, sizeof(dataTX));
	if (dataRX.empty()) {
		*is_new_data = false;
		return;
	}
	uint8_t* dataCAN = dataRX.data();

	// Check if the CAN message back has extended ID
	if (dataCAN[0] == CAN_ID_STD) {
		*is_new_data = false;
	}
	else {
		*ID = (dataCAN[1] << 24) | (dataCAN[2] << 16) | (dataCAN[3] << 8) | dataCAN[4];
		uint8_t DLC = dataCAN[5];
		for (int i = 0; i < DLC; i++) {
			data[i] = dataCAN[6 + i];
		}
		*is_new_data = true;
	}
}

void Tools_Hardware_USB_Protocol_OpenSAEJ1939_callbackFunctionTraffic(uint32_t ID, uint8_t DLC, uint8_t data[], bool is_TX) {
	
}