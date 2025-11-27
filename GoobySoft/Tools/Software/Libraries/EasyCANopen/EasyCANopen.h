#ifndef EasyCANOpen
#define EasyCANOpen

#include "Easy-CANopen/Src/Easy_CANopen/Easy_CANopen.h" 
#include <vector>
#include <string>

void Tools_Software_Libraries_EasyCANopen_callbackFunctionSend(uint16_t COB_ID, uint8_t DLC, uint8_t data[]);
void Tools_Software_Libraries_EasyCANopen_callbackFunctionRead(uint16_t* COB_ID, uint8_t data[], bool* is_new_data);
void Tools_Software_Libraries_EasyCANopen_callbackFunctionTraffic(uint16_t COB_ID, uint8_t DLC, uint8_t data[], bool is_TX);
void Tools_Software_Libraries_EasyCANopen_callbackDelay(uint8_t delay);
std::vector<std::vector<std::string>>& Tools_Software_Libraires_EasyCANopen_getCANTrafficRows();
int* Tools_Software_Libraires_EasyCANopen_getCANTrafficAllowedRows();
void Tools_Software_Libraries_EasyCANopen_setPort(const char port[]);

#endif // !EasyCANOpen
