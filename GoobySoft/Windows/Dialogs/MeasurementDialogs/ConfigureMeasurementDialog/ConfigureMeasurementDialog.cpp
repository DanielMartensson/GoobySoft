#include "ConfigureMeasurementDialog.h"
#include "../../../../Tools/Tools.h"

void Windows_Dialogs_MeasurementDialogs_ConfigureMeasurementDialog_showConfigureMeasurementDialog(bool* configureMeasurement) {
	// Display
	if (ImGui::Begin("Configure measurement", configureMeasurement)) {
		// Get the parameter holder
		Protocol* protocols = Tools_Hardware_ParameterStore_getParameterHolder()->protocols;

		// Create tables inside collapsing headers
		for (int i = 0; i < MAX_PROTOCOLS; i++) {
			// Check if the protocols is available
			if (protocols[i].isProtocolUsed) {
				// Create a collapsing header
				if (ImGui::CollapsingHeader(protocols[i].protocolName)) {
					// Iterate the devices
					Device* devices = protocols[i].devices;
					int deviceCount = protocols[i].deviceCount;
					for (int j = 0; j < deviceCount; j++) {
						// Create a collapsing header with that devices name
						if (ImGui::CollapsingHeader(devices[j].deviceName)) {
							// The table rows of the device
							TableRow* tableRows = devices[j].tableRows;
							int* selectedRowIndex = &devices[j].selectedRowIndex;
							int* tableRowCount = &devices[j].tableRowCount;
							
							// Create buttons
							const std::string deviceName = std::string(devices[j].deviceName);
							if (ImGui::Button(("Add new row of " + deviceName).c_str())) { 
								devices[j].tableRowSelected = tableRows[*selectedRowIndex];
								ImGui::OpenPopup(("popUpAdd" + deviceName).c_str()); 
							}
							if (*tableRowCount > 0) {
								ImGui::SameLine();
								if (ImGui::Button(("Change row of " + deviceName).c_str())) { 
									devices[j].tableRowSelected = tableRows[*selectedRowIndex];
									ImGui::OpenPopup(("popUpChange" + deviceName).c_str()); 
								}
								ImGui::SameLine();
								if (ImGui::Button(("Delete row of " + deviceName).c_str())) { 
									ImGui::OpenPopup(("popUpDelete" + deviceName).c_str()); 
								}
							}
							 
							// Add popup
							if (Tools_Gui_CreateDialogs_showPopUpComboInputIntInputFloatInputTextDialogOKCancle(("popUpAdd" + deviceName).c_str(), &devices[j].tableRowSelected)) {
								if (*tableRowCount > 0) {
									devices[j].tableRowSelected.isHeader = false;
								}
								tableRows[*tableRowCount] = devices[j].tableRowSelected;
								*tableRowCount += 1; // This will add to the bottom of the table, because selectedRowIndex == 0 is the header
							}

							// Change popup
							if (Tools_Gui_CreateDialogs_showPopUpComboInputIntInputFloatInputTextDialogOKCancle(("popUpChange" + deviceName).c_str(), &devices[j].tableRowSelected)) {
								tableRows[*selectedRowIndex] = devices[j].tableRowSelected;
							}

							// Delete popup
							if (Tools_Gui_CreateDialogs_showPopUpConfirmDialogOKCancle(("popUpDelete" + deviceName).c_str(), "Do you want to delete?", nullptr)) {
								for (int i = *selectedRowIndex; i < *tableRowCount; i++) {
									tableRows[i] = tableRows[i + 1];
								}
								*tableRowCount -= 1;
								tableRows[0].isHeader = true; // If first row got replaced, then it must have a header
							}

							// Create table
							std::vector<std::vector<std::string>> rows;

							// Iterate the rows
							for (int k = 0; k < *tableRowCount; k++) {
								std::vector<std::string> row;
								TableColumn* tableColumns = tableRows[k].tableColumns;
								int tableColumnCount = tableRows[k].tableColumnCount;
								if (tableRows[k].isHeader) {
									for (int l = 0; l < tableColumnCount; l++) {
										row.emplace_back(tableColumns[l].tableColumnID.columnName);
									}
									rows.emplace_back(row);
									row.clear();
								}
								for (int l = 0; l < tableColumnCount; l++) {
									switch (tableColumns[l].tableColumnID.columnType) {
									case COLUMN_TYPE_FLOAT:
										row.emplace_back(std::to_string(tableColumns[l].cellValueFloat));
										break;
									case COLUMN_TYPE_INT:
										row.emplace_back(std::to_string(tableColumns[l].cellValueInt));
										break;
									case COLUMN_TYPE_STRING:
										row.emplace_back(tableColumns[l].cellValueString);
										break;
									case COLUMN_TYPE_COMBO:
										row.emplace_back(tableColumns[l].cellValueString);
										break;
									}
								}
								rows.emplace_back(row);
							}
							Tools_Gui_CreateItems_createTableSelectable(("configurationTable" + std::string(deviceName)).c_str(), rows, selectedRowIndex);
						}
					}
				}
			}
		}
		ImGui::End();
	}
}