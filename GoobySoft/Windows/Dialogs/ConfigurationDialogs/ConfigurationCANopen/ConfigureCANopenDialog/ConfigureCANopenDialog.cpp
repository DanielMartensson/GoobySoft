#include "ConfigureCANopenDialog.h"
#include "../../../../../Tools/Tools.h"

void Windows_Dialogs_ConfigurationiDialogs_ConfigurationCANopen_ConfigureCANopenDialog_showConfigureDialog(bool* configureCANopenDialog) {
	// Display
	ImGui::SetNextWindowSize(ImVec2(500, 320));
	if (ImGui::Begin("Configuration CANopen", configureCANopenDialog, ImGuiWindowFlags_NoResize)) {
        // Get connected ports
        static std::string connectedPorts;
		if (ImGui::Button("Scan connected USB ports")) {
			connectedPorts.clear();
			connectedPorts = Tools_Hardware_USB_getConnectedPorts();
		}

        // Create combo box
        static int portIndex = 0;
        ImGui::PushItemWidth(60);
        if(ImGui::Combo("Select USB port", &portIndex, connectedPorts.c_str())){
            // Get selected port
            char port[20];
            Tools_Software_Algorithms_extractElementFromCharArray(connectedPorts.c_str(), portIndex, port);

            // Attach
            Tools_Software_Libraries_EasyCANopen_setPort(port);
        }

        ImGui::LabelText("This node:", nullptr);

		ImGui::End();
	}
}