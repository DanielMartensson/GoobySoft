#include "ConfigureCANopenUSBportDialog.h"
#include "../../../../../Tools/Tools.h"

void Windows_Dialogs_ConfigurationiDialogs_ConfigurationCANopen_ConfigureCANopenUSBportDialog_showConfigureDialog(bool* configureCANopenUSBportDialog) {
	// Display
	if (ImGui::Begin("Configuration USB port", configureCANopenUSBportDialog, ImGuiWindowFlags_AlwaysAutoResize)) {
        // Get connected ports
        static std::string connectedPorts;
		if (ImGui::Button("Scan connected USB ports")) {
			connectedPorts.clear();
			connectedPorts = Tools_Hardware_USB_getConnectedPorts();
		}

        // Create combo box
        static int portIndex = 0;
        ImGui::PushItemWidth(60);
        ImGui::SameLine();
        if(ImGui::Combo("Select USB port", &portIndex, connectedPorts.c_str())){
            // Get selected port
            char port[20];
            Tools_Software_Algorithms_extractElementFromCharArray(connectedPorts.c_str(), portIndex, port);

            // Attach
            Tools_Software_Libraries_EasyCANopen_setPort(port);
        }

		ImGui::End();
	}
}