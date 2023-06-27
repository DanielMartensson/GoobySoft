#include "ConfigureMeasurementDialog.h"
#include "../../../../Tools/Tools.h"

void addChangeDeleteButtons(const char popUpIdAdd[], const char popUpIdChange[], const char popUpIdDelete[]) {
	if (ImGui::Button("Add")) { ImGui::OpenPopup(popUpIdAdd); }
	ImGui::SameLine();
	if (ImGui::Button("Change")) { ImGui::OpenPopup(popUpIdChange); }
	ImGui::SameLine();
	if (ImGui::Button("Delete")) { ImGui::OpenPopup(popUpIdDelete); }
}

void Windows_Dialogs_MeasurementDialogs_ConfigureMeasurementDialog_showConfigureMeasurementDialog(bool* configureMeasurement) {
	// Display
	if (ImGui::BeginPopupModal("Configure measurement", configureMeasurement)) {
		if (ImGui::CollapsingHeader("CDC")) {
			// Get the ports 
			std::vector<std::string> ports = Tools_Hardware_USB_Protocols_CDC_getPortsOfConnectedDevices();

			if (ImGui::CollapsingHeader("STM32PLC")) {

			}
			// Add more...
		}
		if (ImGui::CollapsingHeader("Modbus")) {
			// Get the ports 
			std::vector<std::string> ports = Tools_Hardware_USB_Protocols_Modbus_getPortsOfConnectedDevices();
			if (ImGui::CollapsingHeader("ADL400")) {
				// Table stuffs
				std::vector<std::vector<std::string>>& configurationTable = Tools_Communications_Devices_ADL400_getConfigurationTable();
				std::vector<std::string> labels = configurationTable.at(0);
				std::vector<std::vector<std::string>> items = { ports, SLAVE_ADDRESSES_ADL400, FUNCTION_NAMES_ADL400 };
				static std::vector<std::string> selectedItems(4); // ports, slave address, function names, display name(last element as InputText)
				static size_t selectedRow = 0;

				// Add change delete button
				addChangeDeleteButtons("addADL400", "changeADL400", "deleteADL400");

				// If we pressed ADD button
				int howManyComboBoxes = (int)labels.size() - 1U; // The last element of labels is Display Name. We want that as an InputText box
				if (Tools_Gui_CreateDialogs_showPopUpComboInputTextMultiDialogOKCancle("addADL400", howManyComboBoxes, labels, items, selectedItems)) {
					configurationTable.push_back(selectedItems);
				}

				// If we pressed either change or delete button. The header should not be removed
				if (configurationTable.size() > 1) {
					// Change row
					if (Tools_Gui_CreateDialogs_showPopUpComboInputTextMultiDialogOKCancle("changeADL400", howManyComboBoxes, labels, items, selectedItems)) {
						configurationTable.at(selectedRow) = selectedItems;
					}

					// Delete row
					if (Tools_Gui_CreateDialogs_showPopUpConfirmDialogOKCancle("deleteADL400", "Do you want to delete?", nullptr)) {
						configurationTable.erase(configurationTable.begin() + selectedRow);
					}
				}

				// Create table
				Tools_Gui_CreateItems_createTableSelectable("configurationTableADL400", configurationTable, &selectedRow);
			}
			// Add more...
		}
		// Add more...

		ImGui::EndPopup();
	}
}
