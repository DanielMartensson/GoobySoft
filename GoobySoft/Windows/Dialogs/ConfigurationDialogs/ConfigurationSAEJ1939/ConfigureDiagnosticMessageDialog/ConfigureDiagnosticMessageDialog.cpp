#include "ConfigureDiagnosticMessageDialog.h"
#include "../../../../../Tools/Tools.h"

void Windows_Dialogs_ConfigurationiDialogs_ConfigurationSAEJ1939_ConfigureDiagnosticMessageDialog_showConfigureDiagnosticMessageDialog(bool* configureSAEJ1939DiagnosticMessage) {
	// Display
	ImGui::SetNextWindowSize(ImVec2(500, 480));
	if (ImGui::Begin("Configure SAE J1939 diagnostic messages", configureSAEJ1939DiagnosticMessage, ImGuiWindowFlags_NoResize)) {
		// Get the J1939 struct
		J1939* j1939 = Tools_Hardware_ParameterStore_getJ1939();

		// Select if we want to se this ECU or from other ECU
		static int thisOrOtherECU = 1;
		static int DMmessageType = 1;
		static int DA = 0;
		char text[255];
		ImGui::RadioButton("This ECU", &thisOrOtherECU, 1);
		ImGui::SameLine();
		ImGui::RadioButton("From other ECU", &thisOrOtherECU, 0);
		ImGui::RadioButton("DM1", &DMmessageType, 1);
		ImGui::SameLine();
		ImGui::RadioButton("DM2", &DMmessageType, 2);
		ImGui::SameLine();
		ImGui::RadioButton("DM3", &DMmessageType, 3);
		ImGui::SameLine();
		ImGui::RadioButton("DM16", &DMmessageType, 16);
		ImGui::PushItemWidth(250);
		if (ImGui::InputInt("Destination address", &DA)) {
			if (DA > 255) {
				DA = 255;
			}
			if (DA < 0) {
				DA = 0;
			}
		}

		ImGuiInputTextFlags readOnlyFlag = thisOrOtherECU ? ImGuiInputTextFlags_None : ImGuiInputTextFlags_ReadOnly;

		// Ask for request 
		if (!thisOrOtherECU) {
			switch (DMmessageType) {
			case 1:
				if (ImGui::Button("Request DM1")) {
					SAE_J1939_Send_Request_DM1(j1939, DA);
				}
				break;
			case 2:
				if (ImGui::Button("Request DM2")) {
					SAE_J1939_Send_Request_DM1(j1939, DA);
				}
				break;
			case 3:
				if (ImGui::Button("Request DM3")) {
					SAE_J1939_Send_Request_DM1(j1939, DA);
				}
				break;
			}
		}

		// DM index
		static int dmIndex = 0;
		if (DMmessageType == 1 || DMmessageType == 2) {
			if (ImGui::InputInt("DM index", &dmIndex)) {
				if (dmIndex > (int)MAX_DM_FIELD - 1) {
					dmIndex = (int)MAX_DM_FIELD - 1;
				}
				if (dmIndex < 0) {
					dmIndex = 0;
				}
			}
		}

		ImGui::Separator();

		// DM struct
		DM1* dm = nullptr;
		if (thisOrOtherECU) {
			switch (DMmessageType) {
			case 1:
				dm = &j1939->this_dm.dm1;
				break;
			case 2:
				dm = &j1939->this_dm.dm2;
				break;
			default:
				dm = &j1939->this_dm.dm1;
			}
		}
		else {
			switch (DMmessageType) {
			case 1:
				dm = &j1939->from_other_ecu_dm.dm1;
			case 2:
				dm = &j1939->from_other_ecu_dm.dm2;
			default:
				dm = &j1939->from_other_ecu_dm.dm1;
			}
		}

		if (DMmessageType == 1 || DMmessageType == 2) {
			// Show errors
			uint8_t* errors_dm1_active = thisOrOtherECU ? &j1939->from_other_ecu_dm.errors_dm1_active : &j1939->from_other_ecu_dm.errors_dm1_active;
			uint8_t* errors_dm2_active = thisOrOtherECU ? &j1939->from_other_ecu_dm.errors_dm2_active : &j1939->from_other_ecu_dm.errors_dm2_active;
			uint8_t* errors_active = DMmessageType ? errors_dm1_active : errors_dm2_active;
			ImGui::InputInt("Errors active: ", (int*)errors_active, 1, 100, readOnlyFlag);
			if (thisOrOtherECU) {
				ImGui::Checkbox("SAE lamp status malfunction indicator", (bool*)&dm->SAE_lamp_status_malfunction_indicator);
				ImGui::Checkbox("SAE lamp status red stop", (bool*)&dm->SAE_lamp_status_red_stop);
				ImGui::Checkbox("SAE lamp status amber warning", (bool*)&dm->SAE_lamp_status_amber_warning);
				ImGui::Checkbox("SAE lamp status protect lamp", (bool*)&dm->SAE_lamp_status_protect_lamp);
				ImGui::Checkbox("SAE flash lamp malfunction indicator", (bool*)&dm->SAE_flash_lamp_malfunction_indicator);
				ImGui::Checkbox("SAE flash lamp red stop", (bool*)&dm->SAE_flash_lamp_red_stop);
				ImGui::Checkbox("SAE flash lamp amber warning", (bool*)&dm->SAE_flash_lamp_amber_warning);
				ImGui::Checkbox("SAE flash lamp protect lamp", (bool*)&dm->SAE_flash_lamp_protect_lamp);
				ImGui::Checkbox("SPN conversion method", (bool*)&dm->SPN_conversion_method[dmIndex]);
			}
			else {
				std::sprintf(text, "%s", dm->SAE_lamp_status_malfunction_indicator ? "Yes" : "No");
				ImGui::InputText("SAE lamp status malfunction indicator", text, sizeof(text), readOnlyFlag);
				std::sprintf(text, "%s", dm->SAE_lamp_status_red_stop ? "Yes" : "No");
				ImGui::InputText("SAE lamp status red stop", text, sizeof(text), readOnlyFlag);
				std::sprintf(text, "%s", dm->SAE_lamp_status_amber_warning ? "Yes" : "No");
				ImGui::InputText("SAE lamp status amber warning", text, sizeof(text), readOnlyFlag);
				std::sprintf(text, "%s", dm->SAE_lamp_status_protect_lamp ? "Yes" : "No");
				ImGui::InputText("SAE lamp status protect lamp", text, sizeof(text), readOnlyFlag);
				std::sprintf(text, "%s", dm->SAE_flash_lamp_malfunction_indicator ? "Yes" : "No");
				ImGui::InputText("SAE flash lamp malfunction indicator", text, sizeof(text), readOnlyFlag);
				std::sprintf(text, "%s", dm->SAE_flash_lamp_red_stop ? "Yes" : "No");
				ImGui::InputText("SAE flash lamp red stop", text, sizeof(text), readOnlyFlag);
				std::sprintf(text, "%s", dm->SAE_flash_lamp_amber_warning ? "Yes" : "No");
				ImGui::InputText("SAE flash lamp amber warning", text, sizeof(text), readOnlyFlag);
				std::sprintf(text, "%s", dm->SAE_flash_lamp_protect_lamp ? "Yes" : "No");
				ImGui::InputText("SAE flash lamp protect lamp", text, sizeof(text), readOnlyFlag);
				std::sprintf(text, "%s", dm->SPN_conversion_method[dmIndex] ? "Yes" : "No");
				ImGui::InputText("SPN conversion method", text, sizeof(text), readOnlyFlag);
			}
			ImGui::InputInt("SPN", (int*)&dm->SPN[dmIndex], 1, 100, readOnlyFlag);
			ImGui::InputInt("FMI", (int*)&dm->FMI[dmIndex], 1, 100, readOnlyFlag);
			ImGui::InputInt("Occurrence count", (int*)&dm->occurrence_count[dmIndex], 1, 100, readOnlyFlag);
			if (!thisOrOtherECU) {
				ImGui::InputInt("DM - From ECU address", (int*)&dm->from_ecu_address[dmIndex], 1, 100, readOnlyFlag);
			}
		}
		else if (DMmessageType == 16) {
			if (thisOrOtherECU) {
				static char binaryTextData[255];
				ImGui::InputText("Raw binary data", binaryTextData, sizeof(binaryTextData));
				uint8_t number_of_occurences = std::strlen(binaryTextData) + 1;
				if (ImGui::Button("Send raw binary data to destination address")) {
					SAE_J1939_Send_Binary_Data_Transfer_DM16(j1939, DA, number_of_occurences, (uint8_t*)text);
				}
			}
			else {
				std::sprintf(text, "%i", j1939->from_other_ecu_dm.dm16.number_of_occurences);
				ImGui::InputText("Number of occurences", text, sizeof(text), readOnlyFlag);
				std::strcpy(text, (const char*)j1939->from_other_ecu_dm.dm16.raw_binary_data);
				ImGui::InputText("Raw binary data", text, readOnlyFlag);
				ImGui::InputInt("DM - From ECU address", (int*)&j1939->from_other_ecu_dm.dm16.from_ecu_address, 1, 100, readOnlyFlag);
			}
		}

		ImGui::End();
	}
}