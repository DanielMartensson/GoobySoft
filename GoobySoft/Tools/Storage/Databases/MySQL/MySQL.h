#ifndef MySQL
#define MySQL
#include <string>
#include <vector>

typedef enum {
	MYSQL_STATUS_OK = 1,
	MYSQL_STATUS_FAIL,
	MYSQL_STATUS_CONNECTED,
	MYSQL_STATUS_DISCONNECTED,
	MYSQL_STATUS_MISSING_PRIMARY_KEY
}MYSQL_STATUS;

#define MYSQL_PRIMARY_KEY "id"

MYSQL_STATUS Tools_Storage_Databases_MySQL_openConnection(const char host[], int addDevice, const char username[], const char password[], const char schemaName[]);
MYSQL_STATUS Tools_Storage_Databases_MySQL_closeConnection();
MYSQL_STATUS Tools_Storage_Databases_MySQL_isConnected();
MYSQL_STATUS Tools_Storage_Databases_MySQL_useSchema(const char schemaName[]);
MYSQL_STATUS Tools_Storage_Databases_MySQL_createSchema(const char schemaName[]);
void Tools_Storage_Databases_MySQL_dropSchema(const char schemaName[]);
std::vector<std::string> Tools_Storage_Databases_MySQL_getColumnNames(const char tableName[]);
MYSQL_STATUS Tools_Storage_Databases_MySQL_createTable(const char tableName[], const char columNamesWithDataTypes[]);
uint64_t Tools_Storage_Databases_MySQL_getIdWhereOrderByOffset(const char tableName[], const char where[], const char whereValue[], const char orderBy[], const unsigned int offset);
MYSQL_STATUS Tools_Storage_Databases_MySQL_deleteRow(const char tableName[], const char where[], const char whereValue[], const char orderBy[], const unsigned int offset);
std::vector<std::vector<std::string>> Tools_Storage_Databases_MySQL_getRowsWhereOrderByOffset(const char tableName[], const char where[], const char whereValue[], const char orderBy[], const unsigned int offset);
uint64_t Tools_Storage_Databases_MySQL_countRowsWhere(const char tableName[], const char where[], const char whereValue[]);

#endif // !MySQL