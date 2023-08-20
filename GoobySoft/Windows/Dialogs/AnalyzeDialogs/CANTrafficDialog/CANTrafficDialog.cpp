#include "CANTrafficDialog.h"
#include "../../../../Tools/Tools.h"

static std::vector<std::vector<std::string>> rows;
static int allowed_rows = 10;

void Windows_Dialogs_AnalyzeDialogs_CANTrafficDialog_showCANTrafficDialog(bool* canTrafficDialog) {
	// Display
	if (ImGui::Begin("CAN traffic", canTrafficDialog)) {
		// Input int 
		if (ImGui::InputInt("Allowed rows", &allowed_rows)) {
			if (allowed_rows > 50) {
				allowed_rows = 50;
			}
			if (allowed_rows < 1) {
				allowed_rows = 1;
			}
		}

		// Show table
		Tools_Gui_CreateItems_createTable("CAN data", rows);

		// End
		ImGui::End();
	}
}

void Windows_Dialogs_AnalyzeDialogs_CANTrafficDialog_callbackFunctionTraffic(uint32_t ID, uint8_t DLC, uint8_t data[], bool is_TX) {
	// Add header 
	if (rows.size() == 0) {
		std::vector<std::string> header = { "TX/RX", "ID", "DLC", "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7" };
		rows.emplace_back(header);
	}
	
	// Create new row
	char text[20];
	std::vector<std::string> row;
	row.emplace_back(is_TX ? "TX" : "RX");
	sprintf(text, "0x%X", ID);
	row.emplace_back(text);
	sprintf(text, "0x%X", DLC);
	row.emplace_back(text);
	for (int i = 0; i < 8; i++) {
		if (i < DLC) {
			sprintf(text, "0x%X", data[i]);
			row.emplace_back(text);
		}
		else {
			row.emplace_back("0x0");
		}
	}

	// Add the new row 
	rows.emplace_back(row);

	// Check if it's to much rows
	size_t total_rows = rows.size() - 1; // Don't count with the header
	if (total_rows > allowed_rows) {
		rows.erase(rows.begin() + 1, rows.begin() + 1 + total_rows - allowed_rows);
	}
}