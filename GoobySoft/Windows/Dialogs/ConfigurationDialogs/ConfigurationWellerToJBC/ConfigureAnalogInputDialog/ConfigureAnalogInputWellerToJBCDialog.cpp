#include "ConfigureAnalogInputWellerToJBCDialog.h"
#include "../../../../../Tools/Tools.h"

void Windows_Dialogs_ConfigurationDialogs_ConfigurationWellerToJBC_ConfigureAnalogInputDialog_showConfigureAnalogInputDialog(bool* configureAnalogInput) {
    // Display
    if (ImGui::Begin("Configure analog input", configureAnalogInput)) {

        // Table data
        static std::vector<std::vector<std::string>> rows = {
            {"Signal name", "Raw min", "Raw max", "Real min", "Real max"},
            {"Current", "0", "0", "0.0", "0.0"},
            {"Temperature", "0", "0", "0.0", "0.0"},
            {"Setpoint", "0", "0", "0.0", "0.0"}
        };

        // Editable
        static const std::vector<std::vector<bool>> editableCells = {
            {false, false, false, false, false},
            {false, true, true, true, true},
            {false, true, true, true, true},
            {false, true, true, true, true}
        };

        // Input types
        static const std::vector<std::vector<InputType>> inputTypes = {
            {InputType::Text, InputType::Text, InputType::Text, InputType::Text, InputType::Text}, // Headern, endast text
            {InputType::Text, InputType::Int, InputType::Int, InputType::Float, InputType::Float},
            {InputType::Text, InputType::Int, InputType::Int, InputType::Float, InputType::Float},
            {InputType::Text, InputType::Int, InputType::Int, InputType::Float, InputType::Float}
        };

        // Get the connected ports
        std::string connectedPorts = Tools_Hardware_USB_getConnectedPorts();

        // Create combo box
        static int cdcIndex = 0;
        ImGui::PushItemWidth(60);
        ImGui::Combo("Connected USB ports", &cdcIndex, connectedPorts.c_str());

        // Get selected port
        char port[20];
        Tools_Software_Algorithms_extractElementFromCharArray(connectedPorts.c_str(), cdcIndex, port);

        // The address is fixed for the RTU device
        const int address = 1;

        ImGui::SameLine();
        if (ImGui::Button("Receive calibration")) {
            int rawMin[3], rawMax[3];
            float realMin[3], realMax[3];
            const bool status = Tools_Communications_Devices_WellerToJBC_getAnalogInputCalibration(port, address, rawMin, rawMax, realMin, realMax);
            for (int i = 0; i < 3; i++) {
                // Row 3 = Setpoint, 2 = temperature, 1 = current
                rows.at(3 - i).at(1) = std::to_string(rawMin[i]);
                rows.at(3 - i).at(2) = std::to_string(rawMax[i]);
                rows.at(3 - i).at(3) = std::to_string(realMin[i]);
                rows.at(3 - i).at(4) = std::to_string(realMax[i]);
            }
            if (status) {
                ImGui::OpenPopup("Success received operation PopUp");
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Transmit calibration")) {
            // Row 3 = Setpoint, 2 = temperature, 1 = current
            int rawMin[3] = { std::stoi(rows.at(3).at(1)), std::stoi(rows.at(2).at(1)), std::stoi(rows.at(1).at(1)) };
            int rawMax[3] = { std::stoi(rows.at(3).at(2)), std::stoi(rows.at(2).at(2)), std::stoi(rows.at(1).at(2)) };
            float realMin[3] = { std::stof(rows.at(3).at(3)), std::stof(rows.at(2).at(3)), std::stof(rows.at(1).at(3)) };
            float realMax[3] = { std::stof(rows.at(3).at(4)), std::stof(rows.at(2).at(4)), std::stof(rows.at(1).at(4)) };
            if (Tools_Communications_Devices_WellerToJBC_setAnalogInputCalibration(port, address, rawMin, rawMax, realMin, realMax)) {
                ImGui::OpenPopup("Success transmitted operation PopUp");
            }
        }

        // Create table
        Tools_Gui_CreateItems_createTableEditable("Analog input calibration", rows, editableCells, inputTypes);

        // Save parmeters button
        static int operationIndex = 0;
        ImGui::PushItemWidth(300);
        ImGui::Combo("Operation", &operationIndex, "Parameter estimation\0State estimation\0Control feedback\0State estimation & control feedback\0Recompute model\0Save Parameters");
        ImGui::SameLine();
        if (ImGui::Button("Transmit operation")) {
            if(Tools_Communications_Devices_WellerToJBC_setOperation(port, address, operationIndex)){
                ImGui::OpenPopup("Success transmitted operation PopUp");
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Receive operation")) {
            if (Tools_Communications_Devices_WellerToJBC_getOperation(port, address, &operationIndex)) {
                ImGui::OpenPopup("Success received operation PopUp");
            }
        }

        Tools_Gui_CreateDialogs_showPopUpInformationDialogOK("Success transmitted operation PopUp", "Success: Transmitted");
        Tools_Gui_CreateDialogs_showPopUpInformationDialogOK("Success received operation PopUp", "Success: Received");


		// End
		ImGui::End();
	}
}