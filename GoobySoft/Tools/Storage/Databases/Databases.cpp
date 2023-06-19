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

void Tools_Communications_Units_Database_createFileLogTable() {
	const char tableName[] = "fileLogs";
	const char columNamesWithDataTypes[] = "`id` BIGINT NOT NULL AUTO_INCREMENT, `file_created` DATETIME, `file_path` VARCHAR(1024), PRIMARY KEY(`id`)";
	Tools_Storage_Databases_MySQL_createTable(tableName, columNamesWithDataTypes);
}