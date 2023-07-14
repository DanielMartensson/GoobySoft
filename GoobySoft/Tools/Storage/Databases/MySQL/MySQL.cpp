#include "MySQL.h"
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <mysqlx/xdevapi.h>
#else
#include <mysql-cppconn-8/mysqlx/xdevapi.h>
#endif
#include <cstdio>

static mysqlx::Session* connection = nullptr;
static MYSQL_STATUS connectedToDatabase = MYSQL_STATUS_DISCONNECTED;

static std::vector<std::vector<std::string>> getRowsFrom(mysqlx::RowResult& rows, mysqlx::col_count_t columnCount) {
	std::vector<std::string> dataRow;
	std::vector<std::vector<std::string>> table;
	mysqlx::bytes data;
	const mysqlx::byte* first;
	int year;
	int month;
	int date;
	int hour;
	int minute;
	int second;
	int microsecond;
	char text[50];

	for (auto row : rows) {
		for (mysqlx::col_count_t i = 0; i < columnCount; i++) {
			switch (row[i].getType()) {
			case mysqlx::common::Value::UINT64:
				dataRow.push_back(std::to_string(row[i].get<uint64_t>()));
				break;
			case mysqlx::common::Value::INT64:
				dataRow.push_back(std::to_string(row[i].get<int64_t>()));
				break;
			case mysqlx::common::Value::FLOAT:
				dataRow.push_back(std::to_string(row[i].get<float>()));
				break;
			case mysqlx::common::Value::STRING:
				dataRow.push_back(row[i].get<std::string>());
				break;
			case mysqlx::common::Value::RAW:
				data = row[i].getRawBytes();
				first = data.begin();
				year = (first[1] << 7) | (first[0] & 0x7f);
				month = first[2];
				date = first[3];
				hour = first[4];
				minute = first[5];
				second = first[6];
				microsecond = (first[10] << 21) | (first[9] << 14) | (first[8] << 7) | (first[7] & 0x7f);
				std::sprintf(text, "%i-%i-%i %i:%i:%i.%i", year, month, date, hour, minute, second, microsecond);
				dataRow.push_back(text);
				break;
			default:
				break;
			}
		}
		table.push_back(dataRow);
		dataRow.clear();
	}
	return table;
}

MYSQL_STATUS Tools_Storage_Databases_MySQL_openConnection(const char host[], int addDevice, const char username[], const char password[], const char schemaName[]) {
	// Create connection
	try {
		connection = new mysqlx::Session(mysqlx::SessionOption::HOST, host, mysqlx::SessionOption::PORT, addDevice, mysqlx::SessionOption::USER, username, mysqlx::SessionOption::PWD, password);
		connectedToDatabase = connection->getSchema(schemaName, true).existsInDatabase() ? MYSQL_STATUS_CONNECTED : MYSQL_STATUS_DISCONNECTED;
	}
	catch (...) {}

	// Return status
	return Tools_Storage_Databases_MySQL_isConnected();
}

MYSQL_STATUS Tools_Storage_Databases_MySQL_closeConnection() {
	if (connection != nullptr) {
		connection->close();
		delete connection;
		connection = nullptr;
	}
	connectedToDatabase = MYSQL_STATUS_DISCONNECTED;
	return Tools_Storage_Databases_MySQL_isConnected();
}

MYSQL_STATUS Tools_Storage_Databases_MySQL_isConnected() {
	return connectedToDatabase;
}

MYSQL_STATUS Tools_Storage_Databases_MySQL_useSchema(const char schemaName[]) {
	mysqlx::SqlResult result = connection->sql("USE " + std::string(schemaName)).execute();
	return result.getWarningsCount() == 0 ? MYSQL_STATUS_OK : MYSQL_STATUS_FAIL;
}

MYSQL_STATUS Tools_Storage_Databases_MySQL_createSchema(const char schemaName[]) {
	// Check if schema exist
	if (!connection->getSchema(schemaName).existsInDatabase()) {
		// Create schema
		mysqlx::Schema schema = connection->createSchema(schemaName);

		// And then check if the schema exist
		connectedToDatabase = schema.existsInDatabase() ? MYSQL_STATUS_CONNECTED : MYSQL_STATUS_DISCONNECTED;
		if (connectedToDatabase == MYSQL_STATUS_CONNECTED) {
			return Tools_Storage_Databases_MySQL_isConnected();
		}
	}
	else {
		// Select schema
		connectedToDatabase = MYSQL_STATUS_CONNECTED; // Yes, we have connection
	}

	return Tools_Storage_Databases_MySQL_isConnected();
}

void Tools_Storage_Databases_MySQL_dropSchema(const char schemaName[]) {
	connection->dropSchema(schemaName);
}

std::vector<std::string> Tools_Storage_Databases_MySQL_getColumnNames(const char tableName[]) {
	std::vector< std::string> columNames;
	if (Tools_Storage_Databases_MySQL_isConnected() == MYSQL_STATUS_CONNECTED) {
		const mysqlx::Columns& columnsCombo = connection->getDefaultSchema().getTable(tableName).select("*").limit(1).execute().getColumns();
		for (auto& column : columnsCombo) {
			columNames.push_back(column.getColumnName());
		}
	}
	return columNames;
}

MYSQL_STATUS Tools_Storage_Databases_MySQL_createTable(const char tableName[], const char columNamesWithDataTypes[]) {
	std::string query = "CREATE TABLE IF NOT EXISTS " + std::string(tableName) + " (" + std::string(columNamesWithDataTypes) + ")";
	mysqlx::SqlResult result = connection->sql(query).execute();
	return result.getWarningsCount() == 0 ? MYSQL_STATUS_OK : MYSQL_STATUS_FAIL;
}

uint64_t Tools_Storage_Databases_MySQL_getIdWhereOrderByOffset(const char tableName[], const char where[], const char whereValue[], const char orderBy[], const unsigned int offset) {
	mysqlx::RowResult rows = connection->getDefaultSchema().getTable(tableName).select(MYSQL_PRIMARY_KEY).where(std::string(where) + " = " + std::string(whereValue)).orderBy(orderBy).limit(1).offset(offset).execute();
	return std::stoi(getRowsFrom(rows, 1).at(0).at(0)); // We only get one column and one row due to select(Primary Key) and limit = 1
}

MYSQL_STATUS Tools_Storage_Databases_MySQL_deleteRow(const char tableName[], const char where[], const char whereValue[], const char orderBy[], const unsigned int offset) {
	if (Tools_Storage_Databases_MySQL_isConnected() == MYSQL_STATUS_CONNECTED) {
		// Get ID
		uint64_t ID = Tools_Storage_Databases_MySQL_getIdWhereOrderByOffset(tableName, where, whereValue, orderBy, offset);

		// Delete ID
		std::string query = "DELETE FROM " + std::string(tableName) + " WHERE " + MYSQL_PRIMARY_KEY + " = " + std::to_string(ID);
		mysqlx::SqlResult result = connection->sql(query).execute();
		return result.getWarningsCount() == 0 ? MYSQL_STATUS_OK : MYSQL_STATUS_FAIL;
	}
	else {
		return MYSQL_STATUS_DISCONNECTED;
	}
}

std::vector<std::vector<std::string>> Tools_Storage_Databases_MySQL_getRowsWhereOrderByOffset(const char tableName[], const char where[], const char whereValue[], const char orderBy[], const unsigned int offset) {
	std::vector<std::vector<std::string>> tableData;
	if (Tools_Storage_Databases_MySQL_isConnected() == MYSQL_STATUS_CONNECTED) {
		// Select all rows
		mysqlx::RowResult rows = connection->getDefaultSchema().getTable(tableName).select(MYSQL_PRIMARY_KEY).where(std::string(where) + " = " + std::string(whereValue)).orderBy(orderBy).limit(1).offset(offset).execute();
		tableData = getRowsFrom(rows, rows.getColumnCount());
	}
	return tableData;
}

uint64_t Tools_Storage_Databases_MySQL_countRowsWhere(const char tableName[], const char where[], const char whereValue[]) {
	return connection->getDefaultSchema().getTable(tableName).select(MYSQL_PRIMARY_KEY).where(std::string(where) + " = " + std::string(whereValue)).execute().count();
}
