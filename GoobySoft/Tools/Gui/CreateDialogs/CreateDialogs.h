#ifndef CreateDialogs
#define CreateDialogs
#include <string>
#include "../../Software/Libraries/ImGuiFileDialog/ImGuiFileDialog.h"
#include "../../Storage/Parameters/Parameters.h"


bool Tools_Gui_CreateDialogs_showPopUpConfirmDialogOKCancleWithCheckBox(const char popUpId[], const char question[], const char checkBoxConfirm[], void (*confirmCallBack)(void));
bool Tools_Gui_CreateDialogs_showPopUpConfirmDialogOKCancle(const char popUpId[], const char question[], void (*confirmCallBack)(void));
bool Tools_Gui_CreateDialogs_showPopUpInputTextDialogOKCancle(const char popUpId[], const char question[], void (*inputTextCallBack)(const char[]));
void Tools_Gui_CreateDialogs_showPopUpInformationDialogOK(const char popUpId[], const char information[]);
bool Tools_Gui_CreateDialogs_showPopUpComboInputIntInputFloatInputTextDialogOKCancle(const char popUpId[], TableRow* tableRow);
void Tools_Gui_CreateDialogs_showFileDialog(bool* isOpen, bool* isPressedOK, const char fileFilter[], std::string& folderPath, std::string& filePathName, std::string& fileName, ImGuiFileDialogFlags flags);
void Tools_Gui_CreateDialogs_showFolderDialog(bool* isOpen, bool* isPressedOK, std::string& folderPath);

#endif // !CreateImGuiDialogs
