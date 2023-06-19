#include "LogFilePathDialog.h"
#include "../../../../Tools/Tools.h"

bool Windows_Dialogs_FileDialogs_LogFilePathDialog_showSelectFileDialog(bool* selectFile) {
	std::string folderPath;
	std::string filePathName = std::string(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName); // Initial folder path
	bool isPressedOK = false;
	Tools_Gui_CreateDialogs_showFileDialog(selectFile, &isPressedOK, ".csv", folderPath, filePathName, ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ReadOnlyFileNameField);
	if (isPressedOK) {
		strcpy_s(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName, filePathName.c_str());
		strcpy_s(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.folderPath, folderPath.c_str());
	}
	return isPressedOK;
}

bool Windows_Dialogs_FileDialogs_LogFilePathDialog_showCreateFileDialog(bool* createFile) {
	std::string folderPath;
	std::string filePathName = std::string(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName); // Initial folder path
	bool isPressedOK = false;
	Tools_Gui_CreateDialogs_showFileDialog(createFile, &isPressedOK, ".csv", folderPath, filePathName, ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite);
	if (isPressedOK) {
		strcpy_s(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName, filePathName.c_str());
		strcpy_s(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.folderPath, folderPath.c_str());
		Tools_Hardware_FileSystem_createFile(filePathName);
	}
	return isPressedOK;
}

bool Windows_Dialogs_FileDialogs_LogFilePathDialog_showDeleteFileDialog(bool* deleteFileDialog) {
	if (Windows_Dialogs_FileDialogs_LogFilePathDialog_showSelectFileDialog(deleteFileDialog)) {
		Tools_Hardware_FileSystem_deleteFile(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName);
		if (Tools_Hardware_FileSystem_folderIsEmpty(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.folderPath) == FILE_SYSTEM_STATUS_EMPTY) {
			Tools_Hardware_FileSystem_deleteFile(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.folderPath);
		}
		return true;
	}
	return false;
}