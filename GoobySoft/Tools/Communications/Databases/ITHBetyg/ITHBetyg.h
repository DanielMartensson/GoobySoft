#ifndef ITHBetyg
#define ITHBetyg
#include "../../../Tools.h"

bool Tools_Communications_Databases_ITHBetyg_checkIfPersonExist(const char personNumber[]);
std::vector<std::vector<std::string>> Tools_Communications_Databases_ITHBetyg_getCoursersFrom(const char personNumber[]);
std::string Tools_Communications_Databases_ITHBetyg_getFirstNameFrom(const char personNumber[]);
std::string Tools_Communications_Databases_ITHBetyg_getLastnameFrom(const char personNumber[]);

#endif // !ITHBetyg 