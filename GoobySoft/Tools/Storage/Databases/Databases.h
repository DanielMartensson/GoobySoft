#ifndef Databases
#define Databases
#include "MySQL/MySQL.h"

MYSQL_STATUS Tools_Storage_Databases_openConnection(const char host[], int addDevice, const char username[], const char password[], const char schemaName[]);
void Tools_Storage_Databases_closeConnection();
void Tools_Storage_Databases_dropSchema(const char schemaName[]);

#endif // !Databases
