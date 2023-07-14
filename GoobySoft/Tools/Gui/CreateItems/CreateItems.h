#ifndef CreateItems
#define CreateItems 
#include <string>
#include <vector>

// For the colors
#define COLOR_GREEN (ImVec4)ImColor(48, 199, 138)
#define COLOR_RED (ImVec4)ImColor(148, 37, 70)

void Tools_Gui_CreateItems_createCombo(const char label[], const std::vector<std::string>& items, std::string& selectedItem, bool readOnly);
void Tools_Gui_CreateItems_createListBox(const char label[], const float height, const float width, const std::vector<std::string>& items, std::string& selectedItem, size_t* selectedIndex);
bool Tools_Gui_CreateItems_createTableSelectable(const char strId[], std::vector<std::vector<std::string>> rows, int* selectedRow);
void Tools_Gui_CreateItems_createConnectDisconnectButtons(const bool isConnected, bool allFieldSetForConnection, const char connectButtonText[], const char connectionSuccessMessage[], const char connectionFailMessage[], bool (*connectionCallback)(void), const char disconnectButtonText[], const char disconnectionSuccessMessage[], void (*disconnectionCallback)(void));

#endif // !CreateItems