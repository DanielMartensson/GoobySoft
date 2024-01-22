#include "Databases.h"

MYSQL_STATUS Tools_Storage_Databases_openConnection(const char host[], int addDevice, const char username[], const char password[], const char schemaName[]) {
	return Tools_Storage_Databases_MySQL_openConnection(host, addDevice, username, password, schemaName);
}

void Tools_Storage_Databases_closeConnection() {
	Tools_Storage_Databases_MySQL_closeConnection();
}

void Tools_Storage_Databases_dropSchema(const char schemaName[]) {
	Tools_Storage_Databases_MySQL_dropSchema(schemaName);
	Tools_Storage_Databases_closeConnection();
}

std::vector<std::vector<std::string>> Tools_Storage_Databases_getRowsWhere(const char tableName[], const char where[], const char whereValue[]) {
	return Tools_Storage_Databases_MySQL_getRowsWhere(tableName, where, whereValue);
}

std::vector<std::vector<std::string>> Tools_Storage_Databases_getRowsFromQuery(const char query[]) {
	return Tools_Storage_Databases_MySQL_getRowsFromQuery(query);
}