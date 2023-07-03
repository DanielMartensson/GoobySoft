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
		if (ImGui::Button("OK") && checkBox) {
			if (confirmCallBack != nullptr) {
				confirmCallBack();
			}
			ImGui::CloseCurrentPopup();
			checkBox = false;
			isPressedOK = true;
		}
		ImGui::SameLine();
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

bool Tools_Gui_CreateDialogs_showPopUpComboInputIntInputTextMultiDialogOKCancle(const char popUpId[], const std::vector<std::string>& labelsCombo, const std::vector<std::vector<std::string>>& itemsCombo, std::vector<std::string>& selectedItemsCombo, const std::vector<std::string>& labelsInputInt, std::vector<int>& selectedItemsInputInt, const std::vector<std::string>& labelsInputText, std::vector<std::string>& selectedItemsInputText) {
	ImVec2 popUpCenterPosition(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x * 0.5f, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y * 0.5f);
	ImGui::SetNextWindowPos(popUpCenterPosition, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	bool isPressedOK = false;
	if (ImGui::BeginPopup(popUpId, ImGuiWindowFlags_Modal)) {
		// Combo
		for (size_t i = 0; i < labelsCombo.size(); i++) {
			Tools_Gui_CreateItems_createCombo(labelsCombo.at(i).c_str(), itemsCombo.at(i), selectedItemsCombo.at(i), false);
		}
		// Input int
		for (size_t i = 0; i < labelsInputInt.size(); i++) {
			ImGui::InputInt(labelsInputInt.at(i).c_str(), &selectedItemsInputInt.at(i));
		}
		// Input text
		for (size_t i = 0; i < labelsInputText.size(); i++) {
			char text[200];
			std::strcpy(text, selectedItemsInputText.at(i).c_str());
			ImGui::InputText(labelsInputText.at(i).c_str(), text, sizeof(text));
			selectedItemsInputText.at(i) = std::string(text);
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
		ImGuiFileDialog::Instance()->OpenDialog("FileDialogKey", "File dialog", fileFilter, filePathName, 1, nullptr, flags); // fileFilter[] = ".cpp,.h,.hpp" 

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