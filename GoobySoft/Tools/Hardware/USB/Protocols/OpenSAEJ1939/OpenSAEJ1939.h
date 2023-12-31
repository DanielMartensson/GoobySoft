#ifndef OpenSAEJ1939
#define OpenSAEJ1939

#include "Open_SAE_J1939/Open_SAE_J1939.h"
#include "ISO_11783/ISO_11783-7_Application_Layer/Application_Layer.h"

/* Callbacks - The traffic callback is located at CANTrafficDialog.cpp inside the folder AnalyzeDialogs */
void Tools_Hardware_USB_Protocol_OpenSAEJ1939_callbackFunctionSend(uint32_t ID, uint8_t DLC, uint8_t data[]);
void Tools_Hardware_USB_Protocol_OpenSAEJ1939_callbackFunctionRead(uint32_t* ID, uint8_t data[], bool* is_new_data);

#endif // !OpenSAEJ1939
