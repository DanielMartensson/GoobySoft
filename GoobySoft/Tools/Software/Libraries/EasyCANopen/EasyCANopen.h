#ifndef EasyCANOpen
#define EasyCANOpen

#include <vector>
#include <string>

void Tools_Software_Libraries_EasyCANOpen_callbackFunctionSend(uint32_t ID, uint8_t DLC, uint8_t data[]);
void Tools_Software_Libraries_EasyCANOpen_callbackFunctionRead(uint32_t* ID, uint8_t data[], bool* is_new_data);
void Tools_Software_Libraries_EasyCANOpen_callbackFunctionTraffic(uint32_t ID, uint8_t DLC, uint8_t data[], bool is_TX);
void Tools_Software_Libraries_EasyCANOpen_callbackDelay(uint8_t delay);
std::vector<std::vector<std::string>>& Tools_Software_Libraires_EasyCANOpen_getCANTrafficRows();
int* Tools_Software_Libraires_EasyCANOpen_getCANTrafficAllowedRows();

#endif // !EasyCANOpen
