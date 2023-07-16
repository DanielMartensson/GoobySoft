#include "ConfigureSAEJ1939IdentificationsDialog.h"
#include "../../../../Tools/Tools.h"


void Windows_Dialogs_ConfigurationiDialogs_ConfigureSAEJ1939Dialog_showConfigureSAEJ1939IdentificationsDialog(bool* configureSAEJ1939Identifications) {
	// Display
	ImGui::SetNextWindowSize(ImVec2(500, 700));
	if (ImGui::Begin("Configure SAE J1939 identifications", configureSAEJ1939Identifications, ImGuiWindowFlags_NoResize)) {
		// Get the J1939 struct
		J1939* j1939 = Tools_Hardware_ParameterStore_getJ1939();

		/* Read the CAN messages - if it exist */
		Open_SAE_J1939_Listen_For_Messages(j1939);

		// Information about this ECU
		Name* this_ecu_name = &j1939->information_this_ECU.this_name;
		Identifications* this_ecu_identifications = &j1939->information_this_ECU.this_identifications;

		// Information about other ECU
		static int DA = 0;
		char text[30];
		Identifications* from_other_identifications = &j1939->from_other_ecu_identifications;
		Name* from_other_ecu_name = &j1939->from_other_ecu_name;

		// Select if we want to se this ECU or from other ECU
		static int thisOrOtherECU = 1;
		ImGui::RadioButton("This ECU", &thisOrOtherECU, 1);
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
		ImGui::Separator();

		// This ECU address only
		if (thisOrOtherECU) {
			int ecu_address = j1939->information_this_ECU.this_ECU_address;
			if (ImGui::InputInt("ECU address", &ecu_address)) {
				if (ecu_address > 255) {
					ecu_address = 255;
				}
				if (ecu_address < 0) {
					ecu_address = 0;
				}
			}
			j1939->information_this_ECU.this_ECU_address = ecu_address;
		}

		// Name - Identity
		if (!thisOrOtherECU) {
			if (ImGui::Button("Receive Name")) {
				SAE_J1939_Send_Request_Address_Claimed(j1939, DA);
			}
			ImGui::SameLine();
			if (ImGui::Button("Transmit Name")) {
				SAE_J1939_Send_Commanded_Address(j1939, DA, from_other_ecu_name->from_ecu_address, from_other_ecu_name->identity_number, from_other_ecu_name->manufacturer_code, from_other_ecu_name->function_instance, from_other_ecu_name->ECU_instance, from_other_ecu_name->function, from_other_ecu_name->vehicle_system, from_other_ecu_name->arbitrary_address_capable, from_other_ecu_name->industry_group, from_other_ecu_name->vehicle_system_instance);
			}
		}
		int identity_number = thisOrOtherECU ? this_ecu_name->identity_number : from_other_ecu_name->identity_number;
		if (ImGui::InputInt("Identity number", &identity_number)) {
			if (identity_number > 2097151) {
				identity_number = 2097151;
			}
			if (identity_number < 0) {
				identity_number = 0;
			}
		}
		if (thisOrOtherECU) {
			this_ecu_name->identity_number = identity_number;
		}
		else {
			from_other_ecu_name->identity_number = identity_number;
		}

		// Name - Manufacture code
		std::string selectedManufactureCode;
		std::vector<std::string> manufactureCodes = { "Sonceboz", "Grayhill" };
		int manufacturerCode = thisOrOtherECU ? this_ecu_name->manufacturer_code : from_other_ecu_name->manufacturer_code;
		switch (manufacturerCode) {
		case MANUFACTURER_CODE_SONCEBOZ:
			selectedManufactureCode = manufactureCodes.at(0);
			break;
		case MANUFACTURER_CODE_GRAYHILL:
			selectedManufactureCode = manufactureCodes.at(1);
			break;
		}
		Tools_Gui_CreateItems_createCombo("Manufacture code", manufactureCodes, selectedManufactureCode, false);
		int index = Tools_Software_Algorithms_findIndexOf(manufactureCodes, selectedManufactureCode);
		switch (index) {
		case 0:
			if (thisOrOtherECU) {
				this_ecu_name->manufacturer_code = MANUFACTURER_CODE_SONCEBOZ;
			}
			else {
				from_other_ecu_name->manufacturer_code = MANUFACTURER_CODE_SONCEBOZ;
			}
			break;
		case 1:
			if (thisOrOtherECU) {
				this_ecu_name->manufacturer_code = MANUFACTURER_CODE_GRAYHILL;
			}
			else {
				from_other_ecu_name->manufacturer_code = MANUFACTURER_CODE_GRAYHILL;
			}
			break;
		}

		// Name - Function instance
		int function_instance = thisOrOtherECU ? this_ecu_name->function_instance : from_other_ecu_name->function_instance;
		if (ImGui::InputInt("Function instance", &function_instance)) {
			if (function_instance > 31) {
				function_instance = 31;
			}
			if (function_instance < 0) {
				function_instance = 0;
			}
		}
		if (thisOrOtherECU) {
			this_ecu_name->function_instance = function_instance;
		}
		else {
			from_other_ecu_name->function_instance = function_instance;
		}
		
		// Name - ECU instance
		int ECU_instance = thisOrOtherECU ? this_ecu_name->ECU_instance : from_other_ecu_name->ECU_instance;
		if (ImGui::InputInt("ECU instance", &ECU_instance)) {
			if (ECU_instance > 7) {
				ECU_instance = 7;
			}
			if (ECU_instance < 0) {
				ECU_instance = 0;
			}
		}
		if (thisOrOtherECU) {
			this_ecu_name->ECU_instance = ECU_instance;
		}
		else {
			from_other_ecu_name->ECU_instance = ECU_instance;
		}

		// Name - Function
		std::string selectedFunction;
		std::vector<std::string> functions = { "Auxiliary valve control", "VDC module" };
		int function = thisOrOtherECU ? this_ecu_name->function : from_other_ecu_name->function;
		switch (function) {
		case FUNCTION_AUXILIARY_VALVES_CONTROL:
			selectedFunction = functions.at(0);
			break;
		case FUNCTION_VDC_MODULE:
			selectedFunction = functions.at(1);
			break;
		}
		Tools_Gui_CreateItems_createCombo("Function", functions, selectedFunction, false);
		index = Tools_Software_Algorithms_findIndexOf(functions, selectedFunction);
		switch (index) {
		case 0:
			if (thisOrOtherECU) {
				this_ecu_name->function = FUNCTION_AUXILIARY_VALVES_CONTROL;
			}
			else {
				from_other_ecu_name->function = FUNCTION_AUXILIARY_VALVES_CONTROL;
			}
			break;
		case 1:
			if (thisOrOtherECU) {
				this_ecu_name->function = FUNCTION_VDC_MODULE;
			}
			else {
				from_other_ecu_name->function = FUNCTION_VDC_MODULE;
			}
			break;
		}

		// Name - Industry group
		std::string selectedIndustryGroup;
		std::vector<std::string> industryGroups = { "Global", "On highway", "Agricultural and forrestry", "Construction", "Marine", "Industrial control process"};
		int industryGroup = thisOrOtherECU ? this_ecu_name->industry_group : from_other_ecu_name->industry_group;
		switch (industryGroup) {
		case INDUSTRY_GROUP_GLOBAL:
			selectedIndustryGroup = industryGroups.at(0);
			break;
		case INDUSTRY_GROUP_ON_HIGHWAY:
			selectedIndustryGroup = industryGroups.at(1);
			break;
		case INDUSTRY_GROUP_AGRICULTURAL_AND_FORESTRY:
			selectedIndustryGroup = industryGroups.at(2);
			break;
		case INDUSTRY_GROUP_CONSTRUCTION:
			selectedIndustryGroup = industryGroups.at(3);
			break;
		case INDUSTRY_GROUP_MARINE:
			selectedIndustryGroup = industryGroups.at(4);
			break;
		case INDUSTRY_GROUP_INDUSTRIAL_CONTROL_PROCESS:
			selectedIndustryGroup = industryGroups.at(5);
			break;
		}
		Tools_Gui_CreateItems_createCombo("Industry group", industryGroups, selectedIndustryGroup, false);
		index = Tools_Software_Algorithms_findIndexOf(industryGroups, selectedIndustryGroup);
		switch (index) {
		case 0:
			if (thisOrOtherECU) {
				this_ecu_name->industry_group = INDUSTRY_GROUP_GLOBAL;
			}
			else {
				from_other_ecu_name->industry_group = INDUSTRY_GROUP_GLOBAL;
			}
			break;
		case 1:
			if (thisOrOtherECU) {
				this_ecu_name->industry_group = INDUSTRY_GROUP_ON_HIGHWAY;
			}
			else {
				from_other_ecu_name->industry_group = INDUSTRY_GROUP_ON_HIGHWAY;
			}
			break;
		case 2:
			if (thisOrOtherECU) {
				this_ecu_name->industry_group = INDUSTRY_GROUP_AGRICULTURAL_AND_FORESTRY;
			}
			else {
				from_other_ecu_name->industry_group = INDUSTRY_GROUP_AGRICULTURAL_AND_FORESTRY;
			}
			break;
		case 3:
			if (thisOrOtherECU) {
				this_ecu_name->industry_group = INDUSTRY_GROUP_CONSTRUCTION;
			}
			else {
				from_other_ecu_name->industry_group = INDUSTRY_GROUP_CONSTRUCTION;
			}
			break;
		case 4:
			if (thisOrOtherECU) {
				this_ecu_name->industry_group = INDUSTRY_GROUP_MARINE;
			}
			else {
				from_other_ecu_name->industry_group = INDUSTRY_GROUP_MARINE;
			}
			break;
		case 5:
			if (thisOrOtherECU) {
				this_ecu_name->industry_group = INDUSTRY_GROUP_INDUSTRIAL_CONTROL_PROCESS;
			}
			else {
				from_other_ecu_name->industry_group = INDUSTRY_GROUP_INDUSTRIAL_CONTROL_PROCESS;
			}
			break;
		}

		// Name - Vehicle system instance
		int vehicle_system_instance = thisOrOtherECU ? this_ecu_name->vehicle_system_instance : from_other_ecu_name->vehicle_system_instance;
		if (ImGui::InputInt("Vehicle system instance", &vehicle_system_instance)) {
			if (vehicle_system_instance > 15) {
				vehicle_system_instance = 15;
			}
			if (vehicle_system_instance < 0) {
				vehicle_system_instance = 0;
			}
		}
		if (thisOrOtherECU) {
			this_ecu_name->vehicle_system_instance = vehicle_system_instance;
		}
		else {
			from_other_ecu_name->vehicle_system_instance = vehicle_system_instance;
		}

		// Name - Arbitary address capable
		if (thisOrOtherECU) {
			ImGui::Checkbox("Arbitrary address capable", (bool*)&this_ecu_name->arbitrary_address_capable);
		}
		else {
			std::sprintf(text, "%s", from_other_ecu_name->arbitrary_address_capable ? "Yes" : "No");
			ImGui::InputText("Arbitrary address capable", text, sizeof(text), ImGuiInputTextFlags_ReadOnly);
		}
		
		// Name - From other ECU address
		if (!thisOrOtherECU) {
			int fromECUAddress = from_other_ecu_name->from_ecu_address;
			if (ImGui::InputInt("Name - From ECU address", &fromECUAddress)) {
				if (fromECUAddress > 255) {
					fromECUAddress = 255;
				}
				if (fromECUAddress < 0) {
					fromECUAddress = 0;
				}
			}
			from_other_ecu_name->from_ecu_address = fromECUAddress;
		}
		
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