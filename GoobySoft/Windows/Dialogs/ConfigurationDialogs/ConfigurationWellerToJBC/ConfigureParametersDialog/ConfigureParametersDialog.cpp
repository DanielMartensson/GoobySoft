#include "ConfigureParametersDialog.h"
#include "../../../../../Tools/Tools.h"

void Windows_Dialogs_ConfigurationDialogs_ConfigurationWellerToJBC_ConfigureParametersDialog_showConfigureParametersDialog(bool* configureParameters) {
    // Display
    if (ImGui::Begin("Configure parameters", configureParameters)) {

        // Table data
        static std::vector<std::vector<std::string>> rowsCalibration = {
            {"Signal name", "Raw min", "Raw max", "Real min", "Real max"},
            {"Setpoint", "0", "0", "0.0", "0.0"},
            {"Temperature", "0", "0", "0.0", "0.0"},
            {"Current", "0", "0", "0.0", "0.0"}
        };

        static std::vector<std::vector<std::string>> rowsParameters = {
            {"Parameter name", "Value" },
            {"State space A", "0.0" },
            {"State space B", "0.0" },
            {"Kalman qw disturbance", "0.0" },
            {"Kalman rv noise", "0.0" },
            {"Weight qz", "0.0" },
            {"Regularization s", "0.0" },
            {"Slack variable psi", "0.0" },
            {"Integral action alpha", "0.0" },
            {"Anti-windup integral limit", "0.0" },
            {"Umin", "0.0" },
            {"Umax", "0.0" },
            {"Steady-state model error", "0.0" },
            {"Zmax", "0.0" },
            {"Delta umin", "0.0" },
            {"Delta umax", "0.0" },

        };

        // Editable
        static const std::vector<std::vector<bool>> editableCellsCalibration = {
            {false, false, false, false, false},
            {false, true, true, true, true},
            {false, true, true, true, true},
            {false, true, true, true, true}
        };

        static const std::vector<std::vector<bool>> editableCellsParameters = {
            {false, false},
            {false, true},
            {false, true},
            {false, true},
            {false, true},
            {false, true},
            {false, true},
            {false, true},
            {false, true},
            {false, true},
            {false, true},
            {false, true},
            {false, true},
            {false, true},
            {false, true},
            {false, true}
        };

        // Input types
        static const std::vector<std::vector<InputType>> inputTypesCalibration = {
            {InputType::Text, InputType::Text, InputType::Text, InputType::Text, InputType::Text},
            {InputType::Text, InputType::Int, InputType::Int, InputType::Float, InputType::Float},
            {InputType::Text, InputType::Int, InputType::Int, InputType::Float, InputType::Float},
            {InputType::Text, InputType::Int, InputType::Int, InputType::Float, InputType::Float}
        };

        static const std::vector<std::vector<InputType>> inputTypesParameters = {
            {InputType::Text, InputType::Text}, 
            {InputType::Text, InputType::Float},
            {InputType::Text, InputType::Float},
            {InputType::Text, InputType::Float},
            {InputType::Text, InputType::Float},
            {InputType::Text, InputType::Float},
            {InputType::Text, InputType::Float},
            {InputType::Text, InputType::Float},
            {InputType::Text, InputType::Float},
            {InputType::Text, InputType::Float},
            {InputType::Text, InputType::Float},
            {InputType::Text, InputType::Float},
            {InputType::Text, InputType::Float},
            {InputType::Text, InputType::Float},
            {InputType::Text, InputType::Float},
            {InputType::Text, InputType::Float}
        };

        // Get the connected ports
        std::string connectedPorts = Tools_Hardware_USB_getConnectedPorts();

        // Create combo box
        static int portIndex = 0;
        ImGui::PushItemWidth(60);
        ImGui::Combo("Connected USB ports", &portIndex, connectedPorts.c_str());

        // Get selected port
        char port[20];
        Tools_Software_Algorithms_extractElementFromCharArray(connectedPorts.c_str(), portIndex, port);

        // The address is fixed for the RTU device
        const int address = 1;

        ImGui::SameLine();
        if (ImGui::Button("Receive calibration")) {
            int rawMin[3], rawMax[3];
            float realMin[3], realMax[3];
            const bool status = Tools_Communications_Devices_WellerToJBC_getAnalogInputCalibration(port, address, rawMin, rawMax, realMin, realMax);
            for (int i = 0; i < 3; i++) {
                rowsCalibration.at(i+1).at(1) = std::to_string(rawMin[i]);
                rowsCalibration.at(i+1).at(2) = std::to_string(rawMax[i]);
                rowsCalibration.at(i+1).at(3) = std::to_string(realMin[i]);
                rowsCalibration.at(i+1).at(4) = std::to_string(realMax[i]);
            }
            if (status) {
                ImGui::OpenPopup("Success received operation PopUp");
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Transmit calibration")) {
            // Row 3 = Setpoint, 2 = temperature, 1 = current
            int rawMin[3] = { std::stoi(rowsCalibration.at(1).at(1)), std::stoi(rowsCalibration.at(2).at(1)), std::stoi(rowsCalibration.at(3).at(1)) };
            int rawMax[3] = { std::stoi(rowsCalibration.at(1).at(2)), std::stoi(rowsCalibration.at(2).at(2)), std::stoi(rowsCalibration.at(3).at(2)) };
            float realMin[3] = { std::stof(rowsCalibration.at(1).at(3)), std::stof(rowsCalibration.at(2).at(3)), std::stof(rowsCalibration.at(3).at(3)) };
            float realMax[3] = { std::stof(rowsCalibration.at(1).at(4)), std::stof(rowsCalibration.at(2).at(4)), std::stof(rowsCalibration.at(3).at(4)) };
            if (Tools_Communications_Devices_WellerToJBC_setAnalogInputCalibration(port, address, rawMin, rawMax, realMin, realMax)) {
                ImGui::OpenPopup("Success transmitted operation PopUp");
            }
        }

        // Create table
        Tools_Gui_CreateItems_createTableEditable("Analog input calibration", rowsCalibration, editableCellsCalibration, inputTypesCalibration);
        
        // For the parameters
        if (ImGui::Button("Receive parameters")) {
            float A, B, qw, rv, qz, s, psi, alpha, antiwindup, umin, umax, steadyStateModelError, zmax, deltaumin, deltaumax;
            const bool status = Tools_Communications_Devices_WellerToJBC_getParameters(port, address, &A, &B, &qw, &rv, &qz, &s, &psi, &alpha, &antiwindup, &umin, &umax, &steadyStateModelError, &zmax, &deltaumin, &deltaumax);
            rowsParameters.at(1).at(1) = std::to_string(A);
            rowsParameters.at(2).at(1) = std::to_string(B);
            rowsParameters.at(3).at(1) = std::to_string(qw);
            rowsParameters.at(4).at(1) = std::to_string(rv);
            rowsParameters.at(5).at(1) = std::to_string(qz);
            rowsParameters.at(6).at(1) = std::to_string(s);
            rowsParameters.at(7).at(1) = std::to_string(psi);
            rowsParameters.at(8).at(1) = std::to_string(alpha);
            rowsParameters.at(9).at(1) = std::to_string(antiwindup);
            rowsParameters.at(10).at(1) = std::to_string(umin);
            rowsParameters.at(11).at(1) = std::to_string(umax);
            rowsParameters.at(12).at(1) = std::to_string(steadyStateModelError);
            rowsParameters.at(13).at(1) = std::to_string(zmax);
            rowsParameters.at(14).at(1) = std::to_string(deltaumin);
            rowsParameters.at(15).at(1) = std::to_string(deltaumax);
            if (status) {
                ImGui::OpenPopup("Success received operation PopUp");
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Transmit parameters")) {
            float A, B, qw, rv, qz, s, psi, alpha, antiwindup, umin, umax, steadyStateModelError, zmax, deltaumin, deltaumax;
            A = std::stof(rowsParameters.at(1).at(1));
            B = std::stof(rowsParameters.at(2).at(1));
            qw = std::stof(rowsParameters.at(3).at(1));
            rv = std::stof(rowsParameters.at(4).at(1));
            qz = std::stof(rowsParameters.at(5).at(1));
            s = std::stof(rowsParameters.at(6).at(1));
            psi = std::stof(rowsParameters.at(7).at(1));
            alpha = std::stof(rowsParameters.at(8).at(1));
            antiwindup = std::stof(rowsParameters.at(9).at(1));
            umin = std::stof(rowsParameters.at(10).at(1));
            umax = std::stof(rowsParameters.at(11).at(1));
            steadyStateModelError = std::stof(rowsParameters.at(12).at(1));
            zmax = std::stof(rowsParameters.at(13).at(1));
            deltaumin = std::stof(rowsParameters.at(14).at(1));
            deltaumax = std::stof(rowsParameters.at(15).at(1));
            if(Tools_Communications_Devices_WellerToJBC_setParameters(port, address, A, B, qw, rv, qz, s, psi, alpha, antiwindup, umin, umax, steadyStateModelError, zmax, deltaumin, deltaumax)){
                ImGui::OpenPopup("Success transmitted operation PopUp");
            }
        }
        
        // Create table
        Tools_Gui_CreateItems_createTableEditable("Parameters soldering station", rowsParameters, editableCellsParameters, inputTypesParameters);

        // Save operation button
        static int operationIndex = 0;
        ImGui::PushItemWidth(300); 
        ImGui::Combo("Operation", &operationIndex, "Manually control\0Feedback control\0Compute MPC model\0Save registers to memory");
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