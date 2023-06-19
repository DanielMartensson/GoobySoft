#ifndef FileSystem
#define FileSystem
#include <string>
#include <vector>

typedef enum {
	FILE_SYSTEM_STATUS_CREATED,
	FILE_SYSTEM_STATUS_NOT_CREATED,
	FILE_SYSTEM_STATUS_DELETED,
	FILE_SYSTEM_STATUS_NOT_EXIST,
	FILE_SYSTEM_STATUS_EMPTY,
	FILE_SYSTEM_STATUS_ERROR
}FILE_SYSTEM_STATUS;

std::vector<std::string> Tools_Hardware_FileSystem_listAllFileNamesInFolder(std::string folderPath);
std::vector<std::string> Tools_Hardware_FileSystem_listAllFilePathsInFolder(std::string folderPath);
std::vector<std::string> Tools_Hardware_FileSystem_listAllFileSizesInFolder(std::string folderPath);
std::vector<std::string> Tools_Hardware_FileSystem_listAllFileModifiedInFolder(std::string folderPath);
FILE_SYSTEM_STATUS Tools_Hardware_FileSystem_createFolder(std::string folderName);
FILE_SYSTEM_STATUS Tools_Hardware_FileSystem_deleteFile(std::string filePath);
FILE_SYSTEM_STATUS Tools_Hardware_FileSystem_folderIsEmpty(std::string folderPath);
FILE_SYSTEM_STATUS Tools_Hardware_FileSystem_createFile(std::string filePath);

#endif // !FileSystem
