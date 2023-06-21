#include "LogFilePathDialog.h"
#include "../../../../Tools/Tools.h"

bool Windows_Dialogs_FileDialogs_LogFilePathDialog_showSelectFileDialog(bool* selectFile) {
	std::string folderPath;
	std::string fileName;
	std::string filePathName = std::string(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName); // Initial folder path
	bool isPressedOK = false;
	Tools_Gui_CreateDialogs_showFileDialog(selectFile, &isPressedOK, ".csv", folderPath, filePathName, fileName, ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ReadOnlyFileNameField);
	if (isPressedOK) {
		strcpy_s(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName, filePathName.c_str());
		strcpy_s(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.folderPath, folderPath.c_str());
		strcpy_s(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.fileName, fileName.c_str());
	}
	return isPressedOK;
}

bool Windows_Dialogs_FileDialogs_LogFilePathDialog_showCreateFileDialog(bool* createFile) {
	std::string folderPath;
	std::string fileName;
	std::string filePathName = std::string(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName); // Initial folder path
	bool isPressedOK = false;
	Tools_Gui_CreateDialogs_showFileDialog(createFile, &isPressedOK, ".csv", folderPath, filePathName, fileName, ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite);
	if (isPressedOK) {
		strcpy_s(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName, filePathName.c_str());
		strcpy_s(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.folderPath, folderPath.c_str());
		strcpy_s(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.fileName, fileName.c_str());
		Tools_Hardware_FileSystem_createFile(filePathName);
	}
	return isPressedOK;
}

bool Windows_Dialogs_FileDialogs_LogFilePathDialog_showDeleteFileDialog(bool* deleteFileDialog) {
	if (Windows_Dialogs_FileDialogs_LogFilePathDialog_showSelectFileDialog(deleteFileDialog)) {
		Tools_Hardware_FileSystem_deleteFile(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName);
		memset(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.fileName, 0, sizeof(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.fileName)); // Clear the file name as well
		if (Tools_Hardware_FileSystem_folderIsEmpty(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.folderPath) == FILE_SYSTEM_STATUS_EMPTY) {
			Tools_Hardware_FileSystem_deleteFile(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.folderPath); // Empty folder? Just delete too
		}
		return true;
	}
	return false;
}