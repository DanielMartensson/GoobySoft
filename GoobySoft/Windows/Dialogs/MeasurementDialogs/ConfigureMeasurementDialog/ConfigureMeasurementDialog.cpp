#include "ConfigureMeasurementDialog.h"
#include "../../../../Tools/Tools.h"

static void addChangeDeleteButtons(const char deviceName[]) {
	if (ImGui::Button(("Add new row of " + std::string(deviceName)).c_str())) { ImGui::OpenPopup(("popUpAdd" + std::string(deviceName)).c_str()); }
	ImGui::SameLine();
	if (ImGui::Button(("Change row of " + std::string(deviceName)).c_str())) { ImGui::OpenPopup(("popUpChange" + std::string(deviceName)).c_str()); }
	ImGui::SameLine();
	if (ImGui::Button(("Delete row of " + std::string(deviceName)).c_str())) { ImGui::OpenPopup(("popUpDelete" + std::string(deviceName)).c_str()); }
}

static void addDevice(const char deviceName[], size_t* selectedRow, std::vector<std::vector<std::string>>& configurationTable, const std::vector<std::string>& labelsCombo, const std::vector<std::vector<std::string>>& itemsCombo, std::vector<std::string>& selectedItemsCombo, const std::vector<std::string>& labelsInputFloat, std::vector<float>& selectedItemsInputFloat, const std::vector<std::string>& labelsInputText, std::vector<std::string>& selectedItemsInputText) {
	// Add change delete button
	addChangeDeleteButtons(deviceName);

	// If we pressed ADD button
	if (Tools_Gui_CreateDialogs_showPopUpComboInputIntInputTextMultiDialogOKCancle(("popUpAdd" + std::string(deviceName)).c_str(), labelsCombo, itemsCombo, selectedItemsCombo, labelsInputFloat, selectedItemsInputFloat, labelsInputText, selectedItemsInputText)) {
		std::vector<std::string> row;
		for (size_t i = 0; i < selectedItemsCombo.size(); i++) {
			row.push_back(selectedItemsCombo.at(i));
		}
		for (size_t i = 0; i < selectedItemsInputFloat.size(); i++) {
			row.push_back(std::to_string(selectedItemsInputFloat.at(i)));
		}
		for (size_t i = 0; i < selectedItemsInputText.size(); i++) {
			row.push_back(selectedItemsInputText.at(i));
		}
		configurationTable.emplace_back(row);
	}

	// If we pressed either change or delete button. The header should not be removed
	if (configurationTable.size() > 1) {
		// Change row
		if (Tools_Gui_CreateDialogs_showPopUpComboInputIntInputTextMultiDialogOKCancle(("popUpChange" + std::string(deviceName)).c_str(), labelsCombo, itemsCombo, selectedItemsCombo, labelsInputFloat, selectedItemsInputFloat, labelsInputText, selectedItemsInputText)) {
			std::vector<std::string> row;
			for (size_t i = 0; i < selectedItemsCombo.size(); i++) {
				row.push_back(selectedItemsCombo.at(i));
			}
			for (size_t i = 0; i < selectedItemsInputFloat.size(); i++) {
				row.push_back(std::to_string(selectedItemsInputFloat.at(i)));
			}
			for (size_t i = 0; i < selectedItemsInputText.size(); i++) {
				row.push_back(selectedItemsInputText.at(i));
			}
			configurationTable.at(*selectedRow) = row;
		}

		// Delete row
		if (Tools_Gui_CreateDialogs_showPopUpConfirmDialogOKCancle(("popUpDelete" + std::string(deviceName)).c_str(), "Do you want to delete?", nullptr)) {
			configurationTable.erase(configurationTable.begin() + *selectedRow);
		}
	}

	// Create table
	Tools_Gui_CreateItems_createTableSelectable(("configurationTable" + std::string(deviceName)).c_str(), configurationTable, selectedRow);
}

void Windows_Dialogs_MeasurementDialogs_ConfigureMeasurementDialog_showConfigureMeasurementDialog(bool* configureMeasurement) {
	// Display
	if (ImGui::Begin("Configure measurement", configureMeasurement)) {
		if (ImGui::CollapsingHeader("CDC")) {
			// Get the ports of CDC
			std::vector<std::string> ports = Tools_Hardware_USB_Protocols_CDC_getPortsOfConnectedDevices();
			
			// Create the devices
			if (ImGui::CollapsingHeader("STM32 PLC")) {
				// Table stuffs
				std::vector<std::vector<std::string>>& configurationTable = Tools_Communications_Devices_STM32PLC_getConfigurationTable();
				std::vector<std::string> labels = configurationTable.at(0);

				// Combo
				std::vector<std::string> labelsCombo = { labels.at(0), labels.at(1) }; // Port, Function names
				std::vector<std::vector<std::string>> itemsCombo = { ports, FUNCTION_NAMES_STM32PLC };
				static std::vector<std::string> selectedItemsCombo(itemsCombo.size());

				// Input int
				std::vector<std::string> labelsInputInt = { labels.at(2), labels.at(3) , labels.at(4), labels.at(5) }; // Min value raw, Max value raw, Min value, Max value
				static std::vector<float> selectedItemsInputInt(labelsInputInt.size());

				// Input text
				std::vector<std::string> labelsInputText = { labels.at(6) }; // Display name
				static std::vector<std::string> selectedItemsInputText(labelsInputText.size());

				// Selected row
				static size_t selectedRow = 0;

				// Create the device
				addDevice("STM32 PLC", &selectedRow, configurationTable, labelsCombo, itemsCombo, selectedItemsCombo, labelsInputInt, selectedItemsInputInt, labelsInputText, selectedItemsInputText);
			}
			// Create the devices
			if (ImGui::CollapsingHeader("Open SAE J1939")) {
				// Table stuffs
				std::vector<std::vector<std::string>>& configurationTable = Tools_Communications_Devices_OpenSAEJ1939_getConfigurationTable();
				std::vector<std::string> labels = configurationTable.at(0);

				// Combo
				std::vector<std::string> labelsCombo = { labels.at(0), labels.at(1), labels.at(2) }; // Port, CAN Addresses, Function names
				std::vector<std::vector<std::string>> itemsCombo = { ports, CAN_ADDRESSES_OpenSAEJ1939, FUNCTION_NAMES_OpenSAEJ1939 };
				static std::vector<std::string> selectedItemsCombo(itemsCombo.size());

				// Input int - Not used with SAE J1939
				std::vector<std::string> labelsInputFloat;
				static std::vector<float> selectedItemsInputFloat;

				// Input text
				std::vector<std::string> labelsInputText = { labels.at(3) }; // Display name
				static std::vector<std::string> selectedItemsInputText(labelsInputText.size());

				// Selected row
				static size_t selectedRow = 0;

				// Create the device
				addDevice("Open SAE J1939", &selectedRow, configurationTable, labelsCombo, itemsCombo, selectedItemsCombo, labelsInputFloat, selectedItemsInputFloat, labelsInputText, selectedItemsInputText);

			}
			// Add more...
		}
		if (ImGui::CollapsingHeader("Modbus")) {
			// Get the ports of Modbus
			std::vector<std::string> ports = Tools_Hardware_USB_Protocols_Modbus_getPortsOfConnectedDevices();
			
			// Create the devices
			if (ImGui::CollapsingHeader("ADL400")) {
				// Table stuffs
				std::vector<std::vector<std::string>>& configurationTable = Tools_Communications_Devices_ADL400_getConfigurationTable();
				std::vector<std::string> labels = configurationTable.at(0);

				// Combo
				std::vector<std::string> labelsCombo = { labels.at(0), labels.at(1), labels.at(2)}; // Port, Modbus addresses, Function names
				std::vector<std::vector<std::string>> itemsCombo = { ports , MODBUS_ADDRESSES_ADL400, FUNCTION_NAMES_ADL400 };
				static std::vector<std::string> selectedItemsCombo(itemsCombo.size());

				// Input int - Not used for ADL400
				std::vector<std::string> labelsInputFloat;
				static std::vector<float> selectedItemsInputFloat;

				// Input text
				std::vector<std::string> labelsInputText = { labels.at(3) }; // Display name
				static std::vector<std::string> selectedItemsInputText(labelsInputText.size());

				// Selected row
				static size_t selectedRow = 0;

				// Create the device
				addDevice("ADL400", &selectedRow, configurationTable, labelsCombo, itemsCombo, selectedItemsCombo, labelsInputFloat, selectedItemsInputFloat, labelsInputText, selectedItemsInputText);

			}
			// Add more...
		}
		// Add more...

		ImGui::End();
	}
}
