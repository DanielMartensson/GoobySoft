#include "ITHBetyg.h"

#define ITH_DATABAS_BETYG_SCHEMA "ith_databas_betyg"
#define PERSON_INFOMRATIONS "dbo_student_local_local"
#define PERSON_NUMBER_COLUMN_NAME "PNr"
#define FIRST_NAME_COLUMN_NAME "FirstName"
#define LAST_NAME_COLUMN_NAME "LastName"


bool Tools_Communications_Databases_ITHBetyg_checkIfPersonExist(const char personNumber[]) {
	if (Tools_Storage_Databases_MySQL_useSchema(ITH_DATABAS_BETYG_SCHEMA)) {
		return Tools_Storage_Databases_MySQL_countRowsWhere(PERSON_INFOMRATIONS, PERSON_NUMBER_COLUMN_NAME, personNumber) > 0;
	}
	else {
		return false;
	}
}

std::vector<std::vector<std::string>> Tools_Communications_Databases_ITHBetyg_getCoursersFrom(const char personNumber[]) {
	std::string query = "SELECT * FROM ith_databas_betyg.dbo_examresult_local Where ExamRegistrationId IN (SELECT ExamRegistrationId FROM ith_databas_betyg.dbo_examregistration Where StudentId IN (SELECT StudentId FROM ith_databas_betyg.dbo_student_local_local Where PNr = '" + std::string(personNumber) + "'))";
	return Tools_Storage_Databases_getRowsFromQuery(query.c_str());
}

std::string Tools_Communications_Databases_ITHBetyg_getFirstNameFrom(const char personNumber[]) {
	return Tools_Storage_Databases_getRowsWhere(PERSON_INFOMRATIONS, PERSON_NUMBER_COLUMN_NAME, personNumber).at(0).at(3);
}

std::string Tools_Communications_Databases_ITHBetyg_getLastnameFrom(const char personNumber[]) {
	return Tools_Storage_Databases_getRowsWhere(PERSON_INFOMRATIONS, PERSON_NUMBER_COLUMN_NAME, personNumber).at(0).at(4);
}