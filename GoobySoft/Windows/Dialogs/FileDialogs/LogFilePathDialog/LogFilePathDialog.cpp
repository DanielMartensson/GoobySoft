#include "LogFilePathDialog.h"
#include "../../../../Tools/Tools.h"
#include <cstring>

bool Windows_Dialogs_FileDialogs_LogFilePathDialog_showSelectFileDialog(bool* selectFile) {
	std::string folderPath;
	std::string fileName;
	FileSettings* fileSettings = &Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings;
	std::string filePathName = std::string(fileSettings->filePathName); // Initial folder path
	bool isPressedOK = false;
	Tools_Gui_CreateDialogs_showFileDialog(selectFile, &isPressedOK, ".csv", folderPath, filePathName, fileName, ImGuiFileDialogFlags_ReadOnlyFileNameField);
	if (isPressedOK) {
		std::strcpy(fileSettings->filePathName, filePathName.c_str());
		std::strcpy(fileSettings->folderPath, folderPath.c_str());
		std::strcpy(fileSettings->fileName, fileName.c_str());
	}
	return isPressedOK;
}

bool Windows_Dialogs_FileDialogs_LogFilePathDialog_showCreateFileDialog(bool* createFile) {
	std::string folderPath;
	std::string fileName;
	FileSettings* fileSettings = &Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings;
	std::string filePathName = std::string(fileSettings->filePathName); // Initial folder path
	bool isPressedOK = false;
	Tools_Gui_CreateDialogs_showFileDialog(createFile, &isPressedOK, ".csv", folderPath, filePathName, fileName, ImGuiFileDialogFlags_ConfirmOverwrite);
	if (isPressedOK) {
		std::strcpy(fileSettings->filePathName, filePathName.c_str());
		std::strcpy(fileSettings->folderPath, folderPath.c_str());
		std::strcpy(fileSettings->fileName, fileName.c_str());
		Tools_Hardware_FileSystem_createFile(filePathName);
	}
	return isPressedOK;
}

bool Windows_Dialogs_FileDialogs_LogFilePathDialog_showDeleteFileDialog(bool* deleteFileDialog) {
	if (Windows_Dialogs_FileDialogs_LogFilePathDialog_showSelectFileDialog(deleteFileDialog)) {
		FileSettings* fileSettings = &Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings;
		Tools_Hardware_FileSystem_deleteFile(fileSettings->filePathName);
		std::memset(fileSettings->fileName, 0, sizeof(fileSettings->fileName)); // Clear the file name as well
		if (Tools_Hardware_FileSystem_folderIsEmpty(fileSettings->folderPath) == FILE_SYSTEM_STATUS_EMPTY) {
			Tools_Hardware_FileSystem_deleteFile(fileSettings->folderPath); // Empty folder? Just delete too
		}
		return true;
	}
	return false;
}