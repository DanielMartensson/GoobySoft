#include "CreateItems.h"
#include "../../Software/Libraries/ImGui/imgui.h"


void Tools_Gui_CreateItems_createCombo(const char label[], const std::vector<std::string>& items, std::string& selectedItem, bool readOnly) {
	if (readOnly) {
		int item = 1;
		char const* items[] = { selectedItem.c_str() };
		ImGui::ListBox(label, &item, items, 1);
	}
	else {
		if (ImGui::BeginCombo(label, selectedItem.c_str())) {
			for (int i = 0; i < items.size(); i++) {
				bool isSelected = items.at(i).compare(selectedItem) == 0; // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(items.at(i).c_str(), isSelected)) {
					selectedItem = std::string(items.at(i));
				}
			}
			ImGui::EndCombo();
		}
	}
}

void Tools_Gui_CreateItems_createListBox(const char label[], const float height, const float width, const std::vector<std::string>& items, std::string& selectedItem, size_t* selectedIndex) {
	ImVec2 listBoxSize(height, width);
	if (ImGui::BeginListBox(label, listBoxSize)) {
		for (int i = 0; i < items.size(); i++) {
			bool is_selected = items.at(i).compare(selectedItem) == 0; // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(items.at(i).c_str(), is_selected)) {
				selectedItem = std::string(items.at(i));
				*selectedIndex = i;
			}
		}
		ImGui::EndListBox();
	}
}

bool Tools_Gui_CreateItems_createTable(const char strId[], std::vector<std::vector<std::string>> rows) {
	size_t columnsCombo = rows.size();
	size_t amountOfRows = 0;
	bool clicked = false;
	if (columnsCombo > 0) {
		amountOfRows = rows.size();
		columnsCombo = rows.at(0).size();
	}
	else {
		return false;
	}
	if (ImGui::BeginTable(strId, (int)columnsCombo, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_BordersH | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInner | ImGuiTableFlags_SizingFixedFit)) {
		// Headers setup - Row index 0
		for (size_t column = 0; column < columnsCombo; column++) {
			ImGui::TableSetupColumn(rows.at(0).at(column).c_str());
		}
		ImGui::TableHeadersRow();

		// The rest of the rows
		for (size_t rowIndex = 1; rowIndex < amountOfRows; rowIndex++) {
			ImGui::TableNextRow();
			for (size_t column = 0; column < columnsCombo; column++) {
				ImGui::TableSetColumnIndex((int)column);
				// It's important to add StringValue##XX then XX=row_column because some items could have the same value 
				std::string cellValueString = rows.at(rowIndex).at(column) + "##" + std::to_string(rowIndex) + "_" + std::to_string(column);
				ImGui::Selectable(cellValueString.c_str(), false);
			}
		}
		ImGui::EndTable();
	}
	return true;
}

bool Tools_Gui_CreateItems_createTableSelectable(const char strId[], std::vector<std::vector<std::string>> rows, int* selectedRow) {
	size_t columnsCombo = rows.size();
	size_t amountOfRows = 0;
	bool clicked = false;
	if (columnsCombo > 0) {
		amountOfRows = rows.size();
		columnsCombo = rows.at(0).size();
	}
	else {
		return false;
	}
	if (ImGui::BeginTable(strId, (int)columnsCombo, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_BordersH | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInner | ImGuiTableFlags_SizingFixedFit)) {
		// Headers setup - Row index 0
		for (size_t column = 0; column < columnsCombo; column++) {
			ImGui::TableSetupColumn(rows.at(0).at(column).c_str());
		}
		ImGui::TableHeadersRow();
		
		// The rest of the rows
		for (size_t rowIndex = 1; rowIndex < amountOfRows; rowIndex++){
			ImGui::TableNextRow();
			for (size_t column = 0; column < columnsCombo; column++){
				ImGui::TableSetColumnIndex((int)column);
				// It's important to add StringValue##XX then XX=row_column because some items could have the same value 
				std::string cellValueString = rows.at(rowIndex).at(column) + "##" + std::to_string(rowIndex) + "_" + std::to_string(column);
				if (ImGui::Selectable(cellValueString.c_str(), *selectedRow == rowIndex - 1)) {
					*selectedRow = rowIndex - 1;
					clicked = true;
				}
			}
		}
		ImGui::EndTable();
	}
	return clicked;
}

void Tools_Gui_CreateItems_createConnectDisconnectButtons(const bool isConnected, bool allFieldSetForConnection, const char connectButtonText[], const char connectionSuccessMessage[], const char connectionFailMessage[], bool (*connectionCallback)(void), const char disconnectButtonText[], const char disconnectionSuccessMessage[], void (*disconnectionCallback)(void)) {
	// Color for connect button
	if (isConnected) {
		ImGui::PushStyleColor(ImGuiCol_Button, COLOR_GREEN);
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Button, COLOR_RED);
	}

	// Connect button action
	if (ImGui::Button(connectButtonText)) {
		if (allFieldSetForConnection) {
			bool status = connectionCallback();
			if (status) {
				ImGui::OpenPopup("Connected");
			}
			else {
				ImGui::OpenPopup("Fail");
			}
		}
	}
	ImGui::PopStyleColor();

	// Pop up for connect button - Center for popup
	ImVec2 popUpCenterPosition(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x * 0.5f, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y * 0.5f);
	ImGui::SetNextWindowPos(popUpCenterPosition, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopup("Connected", ImGuiWindowFlags_Modal)) {
		ImGui::Text(connectionSuccessMessage);
		if (ImGui::Button("OK")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	ImGui::SetNextWindowPos(popUpCenterPosition, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopup("Fail", ImGuiWindowFlags_Modal)) {
		ImGui::Text(connectionFailMessage);
		if (ImGui::Button("OK")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	// Color for disconnect button
	if (isConnected) {
		ImGui::PushStyleColor(ImGuiCol_Button, COLOR_RED);
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Button, COLOR_GREEN);
	}

	// Disconnection
	if (ImGui::Button(disconnectButtonText)) {
		if (isConnected) {
			ImGui::OpenPopup("Disconnected");
		}
	}
	ImGui::PopStyleColor();

	// Pop up for disconnect button
	ImGui::SetNextWindowPos(popUpCenterPosition, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopup("Disconnected", ImGuiWindowFlags_Modal)) {
		ImGui::Text(disconnectionSuccessMessage);
		disconnectionCallback();
		if (ImGui::Button("OK")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}