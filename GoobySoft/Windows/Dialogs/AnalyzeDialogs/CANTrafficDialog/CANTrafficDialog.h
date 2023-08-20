#ifndef CANTrafficDialog
#define CANTrafficDialog
#include <cinttypes>

void Windows_Dialogs_AnalyzeDialogs_CANTrafficDialog_showCANTrafficDialog(bool* canTrafficDialog);
void Windows_Dialogs_AnalyzeDialogs_CANTrafficDialog_callbackFunctionTraffic(uint32_t ID, uint8_t DLC, uint8_t data[], bool is_TX);

#endif // !CANTrafficDialog
