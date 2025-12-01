#include "ConfigureIdentificationsDialog.h"
#include "../../../../../Tools/Tools.h"

void Windows_Dialogs_ConfigurationDialogs_ConfigurationSAEJ1939_ConfigureIdentificationsDialog_showConfigureIdentificationsDialog(bool* configureSAEJ1939Identifications) {
	// Display
	if (ImGui::Begin("Configure SAE J1939 identifications", configureSAEJ1939Identifications, ImGuiWindowFlags_AlwaysAutoResize)) {
		// Get the J1939 struct
		J1939* j1939 = Tools_Hardware_ParameterStore_getJ1939();

		// Information about this ECU
		Identifications* this_ecu_identifications = &j1939->information_this_ECU.this_identifications;

		// Information about other ECU
		static int DA = 0;
		char text[30];
		Identifications* from_other_identifications = &j1939->from_other_ecu_identifications;

		// Select if we want to se this ECU or from other ECU
		static int thisOrOtherECU = 1;
		ImGui::RadioButton("This ECU", &thisOrOtherECU, 1);
		ImGui::SameLine();
		ImGui::RadioButton("From other ECU", &thisOrOtherECU, 0);
		ImGui::PushItemWidth(250);
		if (!thisOrOtherECU) {
			if (ImGui::InputInt("Destination address", &DA)) {
				if (DA > 255) {
					DA = 255;
				}
				if (DA < 0) {
					DA = 0;
				}
			}
		}
		ImGuiInputTextFlags readOnlyFlag = thisOrOtherECU ? ImGuiInputTextFlags_None : ImGuiInputTextFlags_ReadOnly;
		
		// ECU identifications
		ImGui::Separator();
		if (!thisOrOtherECU) {
			if (ImGui::Button("Receive ECU ID")) {
				SAE_J1939_Send_Request_ECU_Identification(j1939, DA);
			}
		}
		ImGui::InputText("ECU location", thisOrOtherECU ? (char*)this_ecu_identifications->ecu_identification.ecu_location : (char*)from_other_identifications->ecu_identification.ecu_location, MAX_IDENTIFICATION, readOnlyFlag);
		ImGui::InputText("ECU part number", thisOrOtherECU ? (char*)this_ecu_identifications->ecu_identification.ecu_part_number : (char*)from_other_identifications->ecu_identification.ecu_part_number, MAX_IDENTIFICATION, readOnlyFlag);
		ImGui::InputText("ECU serial number", thisOrOtherECU ? (char*)this_ecu_identifications->ecu_identification.ecu_serial_number : (char*)from_other_identifications->ecu_identification.ecu_serial_number, MAX_IDENTIFICATION, readOnlyFlag);
		ImGui::InputText("ECU type", thisOrOtherECU ? (char*)this_ecu_identifications->ecu_identification.ecu_type : (char*)from_other_identifications->ecu_identification.ecu_type, MAX_IDENTIFICATION, readOnlyFlag);
		if(!thisOrOtherECU){
			std::sprintf(text, "%i", from_other_identifications->ecu_identification.from_ecu_address);
			ImGui::InputText("ECU ID - From ECU address", text, sizeof(text), ImGuiInputTextFlags_ReadOnly);
		}
		
		// Component identifications
		ImGui::Separator();
		if (!thisOrOtherECU) {
			if (ImGui::Button("Receive component ID")) {
				SAE_J1939_Send_Request_Component_Identification(j1939, DA);
			}
		}
		ImGui::InputText("Component model name", thisOrOtherECU ? (char*)this_ecu_identifications->component_identification.component_model_name : (char*)from_other_identifications->component_identification.component_model_name, MAX_IDENTIFICATION, readOnlyFlag);
		ImGui::InputText("Component product date", thisOrOtherECU ? (char*)this_ecu_identifications->component_identification.component_product_date : (char*)from_other_identifications->component_identification.component_product_date, MAX_IDENTIFICATION, readOnlyFlag);
		ImGui::InputText("Component serial number", thisOrOtherECU ? (char*)this_ecu_identifications->component_identification.component_serial_number : (char*)from_other_identifications->component_identification.component_serial_number, MAX_IDENTIFICATION, readOnlyFlag);
		ImGui::InputText("Component unit name", thisOrOtherECU ? (char*)this_ecu_identifications->component_identification.component_unit_name : (char*)from_other_identifications->component_identification.component_unit_name, MAX_IDENTIFICATION, readOnlyFlag);
		if (!thisOrOtherECU) {
			std::sprintf(text, "%i", from_other_identifications->component_identification.from_ecu_address);
			ImGui::InputText("Component ID - From ECU address", text, sizeof(text), ImGuiInputTextFlags_ReadOnly);
		}
		
		// Software identification
		ImGui::Separator();
		if (!thisOrOtherECU) {
			if (ImGui::Button("Receive software ID")) {
				SAE_J1939_Send_Request_Software_Identification(j1939, DA);
			}
		}
		ImGui::InputText("Software identification", thisOrOtherECU ? (char*)this_ecu_identifications->software_identification.identifications : (char*)from_other_identifications->software_identification.identifications, MAX_IDENTIFICATION, readOnlyFlag);
		if (!thisOrOtherECU) {
			std::sprintf(text, "%i", from_other_identifications->software_identification.from_ecu_address);
			ImGui::InputText("Software ID - From ECU address", text, sizeof(text), ImGuiInputTextFlags_ReadOnly);
		}
	
		// End window 
		ImGui::End();
	}
}