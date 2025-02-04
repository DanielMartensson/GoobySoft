#include "ConfigureParametersDialog.h"
#include "../../../../../Tools/Tools.h"

void Windows_Dialogs_ConfigurationDialogs_ConfigurationWellerToJBC_ConfigureParametersDialog_showConfigureParametersDialog(bool* configureParameters) {
    // Display
    if (ImGui::Begin("Configure parameters", configureParameters)) {

        // Table data
        static std::vector<std::vector<std::string>> rowsCalibration = {
            {"Signal name", "Raw min", "Raw max", "Real min", "Real max"},
            {"Current", "0", "0", "0.0", "0.0"},
            {"Temperature", "0", "0", "0.0", "0.0"},
            {"Setpoint", "0", "0", "0.0", "0.0"}
        };

        static std::vector<std::vector<std::string>> rowsParameters = {
            {"Parameter name", "Value" },
            {"State space A", "0.0" },
            {"State space B", "0.0" },
            {"MPC max output singal Umax", "0.0" },
            {"MPC slack variable S", "0.0" },
            {"MPC integral action I", "0.0" },
            {"MPC regularization λ", "0.0" },
            {"KF noise covariance R", "0.0" },
            {"KF disturbance covariance Q", "0.0" },
            {"KF covariance P", "0.0" }
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
            {InputType::Text, InputType::Float}
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
                rowsCalibration.at(3 - i).at(1) = std::to_string(rawMin[i]);
                rowsCalibration.at(3 - i).at(2) = std::to_string(rawMax[i]);
                rowsCalibration.at(3 - i).at(3) = std::to_string(realMin[i]);
                rowsCalibration.at(3 - i).at(4) = std::to_string(realMax[i]);
            }
            if (status) {
                ImGui::OpenPopup("Success received operation PopUp");
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Transmit calibration")) {
            // Row 3 = Setpoint, 2 = temperature, 1 = current
            int rawMin[3] = { std::stoi(rowsCalibration.at(3).at(1)), std::stoi(rowsCalibration.at(2).at(1)), std::stoi(rowsCalibration.at(1).at(1)) };
            int rawMax[3] = { std::stoi(rowsCalibration.at(3).at(2)), std::stoi(rowsCalibration.at(2).at(2)), std::stoi(rowsCalibration.at(1).at(2)) };
            float realMin[3] = { std::stof(rowsCalibration.at(3).at(3)), std::stof(rowsCalibration.at(2).at(3)), std::stof(rowsCalibration.at(1).at(3)) };
            float realMax[3] = { std::stof(rowsCalibration.at(3).at(4)), std::stof(rowsCalibration.at(2).at(4)), std::stof(rowsCalibration.at(1).at(4)) };
            if (Tools_Communications_Devices_WellerToJBC_setAnalogInputCalibration(port, address, rawMin, rawMax, realMin, realMax)) {
                ImGui::OpenPopup("Success transmitted operation PopUp");
            }
        }

        // Create table
        Tools_Gui_CreateItems_createTableEditable("Analog input calibration", rowsCalibration, editableCellsCalibration, inputTypesCalibration);
        
        // For the parameters
        if (ImGui::Button("Receive parameters")) {
            float A, B, Umax, S, I, lambda, R, Q, P;
            const bool status = Tools_Communications_Devices_WellerToJBC_getParameters(port, address, &A, &B, &Umax, &S, &I, &lambda, &R, &Q, &P);
            rowsParameters.at(1).at(1) = std::to_string(A);
            rowsParameters.at(2).at(1) = std::to_string(B);
            rowsParameters.at(3).at(1) = std::to_string(Umax);
            rowsParameters.at(4).at(1) = std::to_string(S);
            rowsParameters.at(5).at(1) = std::to_string(I);
            rowsParameters.at(6).at(1) = std::to_string(lambda);
            rowsParameters.at(7).at(1) = std::to_string(R);
            rowsParameters.at(8).at(1) = std::to_string(Q);
            rowsParameters.at(9).at(1) = std::to_string(P);
            if (status) {
                ImGui::OpenPopup("Success received operation PopUp");
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Transmit parameters")) {
            float A = std::stof(rowsParameters.at(1).at(1));
            float B = std::stof(rowsParameters.at(2).at(1));
            float Umax = std::stof(rowsParameters.at(3).at(1));
            float S = std::stof(rowsParameters.at(4).at(1));
            float I = std::stof(rowsParameters.at(5).at(1));
            float lambda = std::stof(rowsParameters.at(6).at(1));
            float R = std::stof(rowsParameters.at(7).at(1));
            float Q = std::stof(rowsParameters.at(8).at(1));
            float P = std::stof(rowsParameters.at(9).at(1));
            if(Tools_Communications_Devices_WellerToJBC_setParameters(port, address, A, B, Umax, S, I, lambda, R, Q, P)){
                ImGui::OpenPopup("Success transmitted operation PopUp");
            }
        }
        
        // Create table
        Tools_Gui_CreateItems_createTableEditable("Parameters soldering station", rowsParameters, editableCellsParameters, inputTypesParameters);


        // Save operation button
        static int operationIndex = 0;
        ImGui::PushItemWidth(300);
        ImGui::Combo("Operation", &operationIndex, "Free Run\0Control feedback\0Compute model\0Save Parameters");
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