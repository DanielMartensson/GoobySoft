#ifndef CreateDialogs
#define CreateDialogs
#include <string>
#include <vector>
#include "../../Software/Libraries/ImGuiFileDialog/ImGuiFileDialog.h"

bool Tools_Gui_CreateDialogs_showPopUpConfirmDialogOKCancleWithCheckBox(const char popUpId[], const char question[], const char checkBoxConfirm[], void (*confirmCallBack)(void));
bool Tools_Gui_CreateDialogs_showPopUpConfirmDialogOKCancle(const char popUpId[], const char question[], void (*confirmCallBack)(void));
bool Tools_Gui_CreateDialogs_showPopUpInputTextDialogOKCancle(const char popUpId[], const char question[], void (*inputTextCallBack)(const char[]));
void Tools_Gui_CreateDialogs_showPopUpInformationDialogOK(const char popUpId[], const char information[]);
bool Tools_Gui_CreateDialogs_showPopUpComboInputIntInputTextMultiDialogOKCancle(const char popUpId[], const std::vector<std::string>& labelsCombo, const std::vector<std::vector<std::string>>& itemsCombo, std::vector<std::string>& selectedItemsCombo, const std::vector<std::string>& labelsInputInt, std::vector<int>& selectedItemsInputInt, const std::vector<std::string>& labelsInputText, std::vector<std::string>& selectedItemsInputText);
void Tools_Gui_CreateDialogs_showFileDialog(bool* isOpen, bool* isPressedOK, const char fileFilter[], std::string& folderPath, std::string& filePathName, std::string& fileName, ImGuiFileDialogFlags flags);

#endif // !CreateImGuiDialogs
