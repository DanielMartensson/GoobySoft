#include "CANtrafficCANopenDialog.h"
#include "../../../../Tools/Tools.h"

void Windows_Dialogs_AnalyzeDialogs_CANtrafficCANopenDialog_showCANtrafficDialog(bool* canTrafficCANopenDialog) {
	// Display
	if (ImGui::Begin("CANopen traffic analyzer", canTrafficCANopenDialog, ImGuiWindowFlags_AlwaysAutoResize)) {
		// For Easy-CANopen
		int* allowed_rows = Tools_Software_Libraires_EasyCANopen_getCANTrafficAllowedRowsPtr();
		if (ImGui::InputInt("Allowed rows", allowed_rows)) {
			if (*allowed_rows > 50) {
				*allowed_rows = 50;
			}
			if (*allowed_rows < 1) {
				*allowed_rows = 1;
			}
		}

		// Create table
		std::vector<std::vector<std::string>>& rows = Tools_Software_Libraires_EasyCANopen_getCANTrafficRows();
		Tools_Gui_CreateItems_createTable("Easy-CANopen", rows);

		// End
		ImGui::End();
	}
}