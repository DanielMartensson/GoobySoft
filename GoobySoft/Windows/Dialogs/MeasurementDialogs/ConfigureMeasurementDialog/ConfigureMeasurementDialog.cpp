#include "ConfigureMeasurementDialog.h"
#include "../../../../Tools/Tools.h"

void addChangeDeleteButtons(const char popUpIdAdd[], const char popUpIdChange[], const char popUpIdDelete[]) {
	if (ImGui::Button("Add")) { ImGui::OpenPopup(popUpIdAdd); }
	ImGui::SameLine();
	if (ImGui::Button("Change")) { ImGui::OpenPopup(popUpIdChange); }
	ImGui::SameLine();
	if (ImGui::Button("Delete")) { ImGui::OpenPopup(popUpIdDelete); }
}


void addDevice(std::vector<std::vector<std::string>> &table, const char tableId[], std::vector<std::string> &ports, std::vector<std::string> &labels, std::vector<std::vector<std::string>> &items, std::vector<std::string> &selectedItems, size_t *selectedRow) {
	addChangeDeleteButtons("add", "change", "delete");
	int howManyComboBoxes = (int) labels.size() - 1U; // The last element of labels is Display Name. We want that as an InputText box
	if (Tools_Gui_CreateDialogs_showPopUpComboInputTextMultiDialogOKCancle("add", howManyComboBoxes, labels, items, selectedItems)) {
		table.push_back(selectedItems);
	}
	// The header should not be removed
	if (table.size() > 1) {
		if (Tools_Gui_CreateDialogs_showPopUpComboInputTextMultiDialogOKCancle("change", howManyComboBoxes, labels, items, selectedItems)) {
			table.at(*selectedRow) = selectedItems;
		}
		if (Tools_Gui_CreateDialogs_showPopUpConfirmDialogOKCancle("delete", "Do you want to delete?", nullptr)) {
			table.erase(table.begin() + *selectedRow);
		}
	}
	Tools_Gui_CreateItems_createTableSelectable(tableId, table, selectedRow);
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
				std::vector<std::vector<std::string>>& configurationTable = Tools_Communications_Devices_ADL400_getConfigurationTable();
				std::vector<std::string> labels = configurationTable.at(0);
				std::vector<std::vector<std::string>> items = { ports, SLAVE_ADDRESSES_ADL400, FUNCTION_NAMES_ADL400 };
				static std::vector<std::string> selectedItems(4); // ports, slave address, function names, display name(last element as InputText)
				static size_t selectedRow = 0;
				addDevice(configurationTable, "configurationTableADL400", ports, labels, items, selectedItems, &selectedRow);
			}
			// Add more...
		}
		// Add more...

		ImGui::EndPopup();
	}
}
