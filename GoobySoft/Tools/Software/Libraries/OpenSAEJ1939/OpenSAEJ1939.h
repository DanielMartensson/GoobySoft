#ifndef OpenSAEJ1939
#define OpenSAEJ1939

#include "Open_SAE_J1939/Open_SAE_J1939.h"
#include "ISO_11783/ISO_11783-7_Application_Layer/Application_Layer.h"
#include <vector>
#include <string>

void Tools_Software_Libraries_OpenSAEJ1939_callbackFunctionSend(uint32_t ID, uint8_t DLC, uint8_t data[]);
void Tools_Software_Libraries_OpenSAEJ1939_callbackFunctionRead(uint32_t* ID, uint8_t data[], bool* is_new_data);
void Tools_Software_Libraries_OpenSAEJ1939_callbackFunctionTraffic(uint32_t ID, uint8_t DLC, uint8_t data[], bool is_TX);
void Tools_Software_Libraries_OpenSAEJ1939_callbackDelay(uint8_t delay);
std::vector<std::vector<std::string>>& Tools_Software_Libraires_OpenSAEJ1939_getCANTrafficRows();
int* Tools_Software_Libraires_OpenSAEJ1939_getCANTrafficAllowedRows();

#endif // !OpenSAEJ1939
