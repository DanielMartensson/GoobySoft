#ifndef CreateDialogs
#define CreateDialogs
#include <string>
#include <vector>
#include "../../Software/Libraries/ImGuiFileDialog/ImGuiFileDialog.h"

bool Tools_Gui_CreateDialogs_showPopUpConfirmDialogOKCancleWithCheckBox(const char popUpId[], const char question[], const char checkBoxConfirm[], void (*confirmCallBack)(void));
bool Tools_Gui_CreateDialogs_showPopUpConfirmDialogOKCancle(const char popUpId[], const char question[], void (*confirmCallBack)(void));
bool Tools_Gui_CreateDialogs_showPopUpInputTextDialogOKCancle(const char popUpId[], const char question[], void (*inputTextCallBack)(const char[]));
void Tools_Gui_CreateDialogs_showPopUpInformationDialogOK(const char popUpId[], const char information[]);
bool Tools_Gui_CreateDialogs_showPopUpComboInputTextMultiDialogOKCancle(const char popUpId[], const int amountOfCombos, std::vector<std::string>& labels, std::vector<std::vector<std::string>>& items, std::vector<std::string>& selectedItems);
void Tools_Gui_CreateDialogs_showFileDialog(bool* isOpen, bool* isPressedOK, const char fileFilter[], std::string& folderPath, std::string& filePathName, std::string& fileName, ImGuiFileDialogFlags flags);

#endif // !CreateImGuiDialogs
