#include "CreateDialogs.h"
#include "../../Tools.h"
#include <cstring>

bool Tools_Gui_CreateDialogs_showPopUpConfirmDialogOKCancleWithCheckBox(const char popUpId[], const char question[], const char checkBoxConfirm[], void (*confirmCallBack)(void)) {
	ImVec2 popUpCenterPosition(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x * 0.5f, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y * 0.5f);
	ImGui::SetNextWindowPos(popUpCenterPosition, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	bool isPressedOK = false;
	if (ImGui::BeginPopup(popUpId, ImGuiWindowFlags_Modal)) {
		ImGui::Text(question);
		static bool checkBox = false;
		if (checkBox) {
			if (ImGui::Button("OK")) {
				if (confirmCallBack != nullptr) {
					confirmCallBack();
				}
				ImGui::CloseCurrentPopup();
				checkBox = false;
				isPressedOK = true;
			}
			ImGui::SameLine();
		}
		ImGui::Checkbox(checkBoxConfirm, &checkBox);
		ImGui::SameLine();
		if (ImGui::Button("Cancle")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	return isPressedOK;
}

bool Tools_Gui_CreateDialogs_showPopUpConfirmDialogOKCancle(const char popUpId[], const char question[], void (*confirmCallBack)(void)) {
	ImVec2 popUpCenterPosition(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x * 0.5f, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y * 0.5f);
	ImGui::SetNextWindowPos(popUpCenterPosition, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	bool isPressedOK = false;
	if (ImGui::BeginPopup(popUpId, ImGuiWindowFlags_Modal)) {
		ImGui::Text(question);
		if (ImGui::Button("OK")) {
			if (confirmCallBack != nullptr) {
				confirmCallBack();
			}
			ImGui::CloseCurrentPopup();
			isPressedOK = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancle")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	return isPressedOK;
}

bool Tools_Gui_CreateDialogs_showPopUpInputTextDialogOKCancle(const char popUpId[], const char question[], void (*inputTextCallBack)(const char[])) {
	ImVec2 popUpCenterPosition(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x * 0.5f, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y * 0.5f);
	ImGui::SetNextWindowPos(popUpCenterPosition, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	bool isPressedOK = false;
	if (ImGui::BeginPopup(popUpId, ImGuiWindowFlags_Modal)) {
		ImGui::Text(question);
		static char fileName[100];
		ImGui::InputText(" ", fileName, sizeof(fileName));
		if (ImGui::Button("OK")) {
			if (inputTextCallBack != nullptr) {
				inputTextCallBack(fileName);
			}
			std::memset(fileName, 0, sizeof(fileName));
			isPressedOK = true;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancle")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	return isPressedOK;
}

void Tools_Gui_CreateDialogs_showPopUpInformationDialogOK(const char popUpId[], const char information[]) {
	ImVec2 popUpCenterPosition(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x * 0.5f, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y * 0.5f);
	ImGui::SetNextWindowPos(popUpCenterPosition, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopup(popUpId, ImGuiWindowFlags_Modal)) {
		ImGui::Text(information);
		if (ImGui::Button("OK")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

bool Tools_Gui_CreateDialogs_showPopUpComboInputIntInputFloatInputTextDialogOKCancle(const char popUpId[], TableRow* tableRow) {
	ImVec2 popUpCenterPosition(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x * 0.5f, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y * 0.5f);
	ImGui::SetNextWindowPos(popUpCenterPosition, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	bool isPressedOK = false;
	if (ImGui::BeginPopup(popUpId, ImGuiWindowFlags_Modal)) {
		for (int i = 0; i < tableRow->tableColumnCount; i++) {
			// Iterate the columns
			TableColumn* tableColumn = &tableRow->tableColumns[i];
			switch (tableColumn->tableColumnID.columnType) {
			case COLUMN_TYPE_COMBO:
				ImGui::Combo(tableColumn->tableColumnID.columnName, &tableColumn->functionValueIndex, tableColumn->functionValues);
				Tools_Software_Algorithms_extractElementFromCharArray(tableColumn->functionValues, tableColumn->functionValueIndex, tableColumn->cellValueString);
				// Important to update the column function, dependning on index of function
				if (tableColumn->tableColumnID.columnDefinition == COLUMN_DEFINITION_FUNCTION) {
					tableColumn->tableColumnID.columnFunction = tableRow->getColumnFunction(tableColumn->functionValueIndex);
				}
				break;
			case COLUMN_TYPE_INT: {
				COLUMN_FUNCTION columnFunction = Tools_Hardware_ParameterStore_readColumnFunctionAtColumnDefinition(tableRow->tableColumns, tableRow->tableColumnCount, COLUMN_DEFINITION_FUNCTION);
				switch (columnFunction) {
				case COLUMN_FUNCTION_INPUT_SENSOR_ANALOG:
					// Analog sensor should not have an address input
					if (tableColumn->tableColumnID.columnDefinition == COLUMN_DEFINITION_ADDRESS) {
						break;
					}
					ImGui::InputInt(tableColumn->tableColumnID.columnName, &tableColumn->cellValueInt);
					break;
				case COLUMN_FUNCTION_OUTPUT_ACTUATOR:
					// Output actuator should not have an address input
					if (tableColumn->tableColumnID.columnDefinition == COLUMN_DEFINITION_ADDRESS) {
						break;
					}
					ImGui::InputInt(tableColumn->tableColumnID.columnName, &tableColumn->cellValueInt);
					break;
				case COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS:
					ImGui::InputInt(tableColumn->tableColumnID.columnName, &tableColumn->cellValueInt);
					break;
				case COLUMN_FUNCTION_OUTPUT_ACTUATOR_ADDRESS:
					ImGui::InputInt(tableColumn->tableColumnID.columnName, &tableColumn->cellValueInt);
					break;
				}
				break;
			}
			case COLUMN_TYPE_FLOAT: {
				COLUMN_FUNCTION columnFunction = Tools_Hardware_ParameterStore_readColumnFunctionAtColumnDefinition(tableRow->tableColumns, tableRow->tableColumnCount, COLUMN_DEFINITION_FUNCTION);
				switch (columnFunction) {
				case COLUMN_FUNCTION_INPUT_SENSOR_ANALOG:
					// Analog sensor should not have an address input
					if (tableColumn->tableColumnID.columnDefinition == COLUMN_DEFINITION_ADDRESS) {
						break;
					}
					ImGui::InputFloat(tableColumn->tableColumnID.columnName, &tableColumn->cellValueFloat);
					break;
				}
				break;
			}
			case COLUMN_TYPE_STRING:
				ImGui::InputText(tableColumn->tableColumnID.columnName, tableColumn->cellValueString, sizeof(tableColumn->cellValueString));
				break;
			}
		}

		// Close
		if (ImGui::Button("OK")) {
			isPressedOK = true;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Close")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	return isPressedOK;
}

void Tools_Gui_CreateDialogs_showFileDialog(bool* isOpen, bool* isPressedOK, const char fileFilter[], std::string& folderPath, std::string& filePathName, std::string& fileName, ImGuiFileDialogFlags flags) {
	if (*isOpen) {
		IGFD::FileDialogConfig vConfig;
		vConfig.flags = flags;
		ImGuiFileDialog::Instance()->OpenDialog("FileDialogKey", "File dialog", fileFilter, vConfig);

		// Display
		if (ImGuiFileDialog::Instance()->Display("FileDialogKey"))
		{
			// Action if OK
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
				folderPath = ImGuiFileDialog::Instance()->GetCurrentPath();
				fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
				// Action
				*isPressedOK = true;
			}

			// close
			ImGuiFileDialog::Instance()->Close();
			*isOpen = false;
		}
	}
}

void Tools_Gui_CreateDialogs_showFolderDialog(bool* isOpen, bool* isPressedOK, std::string& folderPath) {
	if (*isOpen) {
		IGFD::FileDialogConfig vConfig;
		ImGuiFileDialog::Instance()->OpenDialog("FolderDialogKey", "Choose a Directory", ".", vConfig);

		// Display
		if (ImGuiFileDialog::Instance()->Display("FolderDialogKey"))
		{
			// Action if OK
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				folderPath = ImGuiFileDialog::Instance()->GetCurrentPath();
				// Action
				*isPressedOK = true;
			}

			// close
			ImGuiFileDialog::Instance()->Close();
			*isOpen = false;
		}
	}
}