#include "FileSystem.h"
#include "boost/filesystem.hpp"
#include <boost/filesystem/fstream.hpp>
#include <ctime>

std::vector<std::string> Tools_Hardware_FileSystem_listAllFileNamesInFolder(std::string folderPath) {
	std::vector<std::string> files;
	boost::filesystem::directory_iterator endIterator;
	for (boost::filesystem::directory_iterator iter(folderPath); iter != endIterator; ++iter) {
		if (boost::filesystem::is_regular_file(iter->status())) {
			files.push_back(iter->path().filename().string());
		}
	}
	return files;
}

std::vector<std::string> Tools_Hardware_FileSystem_listAllFilePathsInFolder(std::string folderPath) {
	std::vector<std::string> paths;
	boost::filesystem::directory_iterator endIterator;
	for (boost::filesystem::directory_iterator iter(folderPath); iter != endIterator; ++iter) {
		if (boost::filesystem::is_regular_file(iter->status())) {
			paths.push_back(iter->path().string());
		}
	}
	return paths;
}

std::vector<std::string> Tools_Hardware_FileSystem_listAllFileSizesInFolder(std::string folderPath) {
	std::vector<std::string> sizes;
	boost::filesystem::directory_iterator endIterator;
	for (boost::filesystem::directory_iterator iter(folderPath); iter != endIterator; ++iter) {
		if (boost::filesystem::is_regular_file(iter->status())) {
			std::string fileSize = std::to_string(boost::filesystem::file_size(iter->path()));
			sizes.push_back(fileSize);
		}
	}
	return sizes;
}

std::vector<std::string> Tools_Hardware_FileSystem_listAllFileModifiedInFolder(std::string folderPath) {
	std::vector<std::string> filesModified;
	boost::filesystem::directory_iterator endIterator;
	for (boost::filesystem::directory_iterator iter(folderPath); iter != endIterator; ++iter) {
		if (boost::filesystem::is_regular_file(iter->status())) {
			std::time_t fileModified = boost::filesystem::last_write_time(iter->path());
			const char* timeText = std::ctime(&fileModified);
			filesModified.push_back(timeText);
		}
	}
	return filesModified;
}


FILE_SYSTEM_STATUS Tools_Hardware_FileSystem_createFolder(std::string folderName) {
	try {
		if (boost::filesystem::create_directory(folderName)) {
			return FILE_SYSTEM_STATUS_CREATED;
		}
		else {
			return FILE_SYSTEM_STATUS_NOT_CREATED;
		}
	}
	catch (...) {
		return FILE_SYSTEM_STATUS_ERROR;
	}
}

FILE_SYSTEM_STATUS Tools_Hardware_FileSystem_deleteFile(std::string filePath) {
	try {
		if (boost::filesystem::remove(filePath)) {
			return FILE_SYSTEM_STATUS_DELETED;
		}
		else {
			return FILE_SYSTEM_STATUS_NOT_EXIST;
		}
	}
	catch (...) {
		return FILE_SYSTEM_STATUS_ERROR;
	}
}

FILE_SYSTEM_STATUS Tools_Hardware_FileSystem_folderIsEmpty(std::string folderPath) {
	try {
		if (boost::filesystem::is_empty(folderPath)) {
			return FILE_SYSTEM_STATUS_EMPTY;
		}
		else {
			return FILE_SYSTEM_STATUS_NOT_EXIST;
		}
	}
	catch (...) {
		return FILE_SYSTEM_STATUS_ERROR;
	}
}

FILE_SYSTEM_STATUS Tools_Hardware_FileSystem_createFile(std::string filePath) {
	try {
		boost::filesystem::ofstream file(filePath);
		if (file.is_open()) {
			return FILE_SYSTEM_STATUS_CREATED;
		}
		else {
			return FILE_SYSTEM_STATUS_NOT_CREATED;
		}
	}
	catch (...) {
		return FILE_SYSTEM_STATUS_ERROR;
	}
}