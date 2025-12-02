#include "ConfigureSAEJ1939USBportDialog.h"
#include "../../../../../Tools/Tools.h"

void Windows_Dialogs_ConfigurationiDialogs_ConfigurationSAEJ1939_ConfigureSAEJ1939USBportDialog_showConfigureDialog(bool* configureSAEJ1939USBportDialog) {
	// Display
	if (ImGui::Begin("Configuration SAE J1939 USB port", configureSAEJ1939USBportDialog, ImGuiWindowFlags_AlwaysAutoResize)) {
        // Get connected ports
        static std::string connectedPorts;
		if (ImGui::Button("Scan connected USB ports")) {
			connectedPorts.clear();
			connectedPorts = Tools_Hardware_USB_getConnectedPorts();
		}

        // Create combo box
        static int portIndex = 0;
        static char port[20] = {0};
        ImGui::SameLine();
        ImGui::Combo("Select USB port", &portIndex, connectedPorts.c_str());
        ImGui::SameLine();
        if(ImGui::Button("Set port")){
            // Get selected port
            Tools_Software_Algorithms_extractElementFromCharArray(connectedPorts.c_str(), portIndex, port);
            if(Tools_Hardware_USB_isConnected(port) == USB_STATUS_CONNECTED){
                // Attach
                Tools_Software_Libraries_OpenSAEJ1939_setPort(port);

                // Open pop-up
                ImGui::OpenPopup("selected SAE J1939 port - connected");
            }else{
                ImGui::OpenPopup("selected SAE J1939 port - not connected");
            } 
        }

        // Show pop-up
        std::string selected_port_message_connected = "Connection success: The port " + std::string(port) + " is selected for SAE J1939!";
        std::string selected_port_message_not_connected = "Connection issue: The port " + std::string(port) + " cannot be connected!";
        Tools_Gui_CreateDialogs_showPopUpInformationDialogOK("selected SAE J1939 port - connected", selected_port_message_connected.c_str());
        Tools_Gui_CreateDialogs_showPopUpInformationDialogOK("selected SAE J1939 port - not connected", selected_port_message_not_connected.c_str());

		ImGui::End();
	}
}