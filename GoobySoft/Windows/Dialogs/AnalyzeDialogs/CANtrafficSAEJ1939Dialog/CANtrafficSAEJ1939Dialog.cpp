#include "CANtrafficSAEJ1939Dialog.h"
#include "../../../../Tools/Tools.h"

void Windows_Dialogs_AnalyzeDialogs_CANtrafficSAEJ1939Dialog_showCANtrafficDialog(bool* canTrafficSAEJ1939Dialog) {
	// Display
	if (ImGui::Begin("SAE J1939 traffic analyzer", canTrafficSAEJ1939Dialog, ImGuiWindowFlags_AlwaysAutoResize)) {
		// For Open SAE J1939 
		int* allowed_rows = Tools_Software_Libraires_OpenSAEJ1939_getCANTrafficAllowedRowsPtr();
		if (ImGui::InputInt("Allowed rows", allowed_rows)) {
			if (*allowed_rows > 50) {
				*allowed_rows = 50;
			}
			if (*allowed_rows < 1) {
				*allowed_rows = 1;
			}
		}

		// Create table
		std::vector<std::vector<std::string>>& rows = Tools_Software_Libraires_OpenSAEJ1939_getCANTrafficRows();
		Tools_Gui_CreateItems_createTable("Open SAE J1939", rows);

		// End
		ImGui::End();
	}
}