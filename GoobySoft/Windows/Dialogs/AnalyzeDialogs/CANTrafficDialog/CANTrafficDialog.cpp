#include "CANTrafficDialog.h"
#include "../../../../Tools/Tools.h"

void Windows_Dialogs_AnalyzeDialogs_CANTrafficDialog_showCANTrafficDialog(bool* canTrafficDialog) {
	// Display
	if (ImGui::Begin("CAN traffic", canTrafficDialog)) {
		// Input int 
		int* allowed_rows = Tools_Software_Libraires_OpenSAEJ1939_getCANTrafficAllowedRows();
		if (ImGui::InputInt("Allowed rows", allowed_rows)) {
			if (*allowed_rows > 50) {
				*allowed_rows = 50;
			}
			if (*allowed_rows < 1) {
				*allowed_rows = 1;
			}
		}

		// Get rows
		std::vector<std::vector<std::string>>& rows = Tools_Software_Libraires_OpenSAEJ1939_getCANTrafficRows();

		// Show table
		Tools_Gui_CreateItems_createTable("CAN data", rows);

		// End
		ImGui::End();
	}
}