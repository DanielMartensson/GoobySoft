#include "GradesDialog.h"
#include "../../../../../Tools/Tools.h"

std::string firstName;
std::string lastName;
std::vector<std::vector<std::string>> course_data;

void Windows_Dialogs_UtilDialogs_DatabaseDialogs_GradesDialog_showViewGradesDialog(bool* viewGradesDialog) {
	// Display
	if (ImGui::Begin("View grades dialog", viewGradesDialog)) {

		// Check if it's connected to the database
		if (Tools_Storage_Databases_MySQL_isConnected() == MYSQL_STATUS_CONNECTED) {
			// Person information
			static char personNumber[100];
			ImGui::InputText("Person number", personNumber, sizeof(personNumber));

			// Searsch
			if (ImGui::Button("Searsch")) {
				if (Tools_Communications_Databases_ITHBetyg_checkIfPersonExist(personNumber)) {
					// Get all information about courses
					std::vector<std::vector<std::string>> courses = Tools_Communications_Databases_ITHBetyg_getCoursersFrom(personNumber);
					firstName = Tools_Communications_Databases_ITHBetyg_getFirstNameFrom(personNumber);
					lastName = Tools_Communications_Databases_ITHBetyg_getLastnameFrom(personNumber);

					// Iterate all rows
					course_data.clear();
					course_data.push_back({ "Course", "Grade", "Points", "Registrerad"});
					for (int i = 0; i < courses.size(); i++) {
						std::string ExamId = "null";
						std::string Registred = "null";
						std::string Points = "null";
						std::string Grade = "null";

						if (courses.at(i).size() == 6) {
							ExamId = courses.at(i).at(2);
							Points = courses.at(i).at(4);
							Grade = courses.at(i).at(5);
						}
						else {
							ExamId = courses.at(i).at(2);
							Registred = courses.at(i).at(4);
							Points = courses.at(i).at(5);
							Grade = courses.at(i).at(6);
						}						

						if (strcmp(ExamId.c_str(), "1") == 0) {
							ExamId = "Grundkurs 1";
						}
						else if (strcmp(ExamId.c_str(), "2") == 0) {
							ExamId = "Grundkurs 2";
						}
						else if (strcmp(ExamId.c_str(), "3") == 0) {
							ExamId = "Kurs för konstruktörer";
						}
						else if (strcmp(ExamId.c_str(), "4") == 0) {
							ExamId = "Kurs för montörer";
						}
						else if (strcmp(ExamId.c_str(), "5") == 0) {
							ExamId = "Kurs för underhållstekniker";
						}
						course_data.push_back({ ExamId, Grade, Points, Registred });
					}
				}
			}

			// Create table
			char text[100];
			sprintf(text, "First name: %s\nLast name: %s", firstName.c_str(), lastName.c_str());
			ImGui::Text(text);
			Tools_Gui_CreateItems_createTable("courses", course_data);	
		}
		else {
			ImGui::Text("Not connected to MySQL database");
		}

		// End
		ImGui::End();
	}
}