#include "ConfigureCANopenUSBportDialog.h"
#include "../../../../../Tools/Tools.h"

void Windows_Dialogs_ConfigurationiDialogs_ConfigurationCANopen_ConfigureCANopenUSBportDialog_showConfigureDialog(bool* configureCANopenUSBportDialog) {
	// Display
	if (ImGui::Begin("Configuration CANopen USB port", configureCANopenUSBportDialog, ImGuiWindowFlags_AlwaysAutoResize)) {
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
                Tools_Software_Libraries_EasyCANopen_setPort(port);

                // Open pop-up
                ImGui::OpenPopup("selected CANopen port - connected");
            }else{
                ImGui::OpenPopup("selected CANopen port - not connected");
            } 
        }

        // Show pop-up
        std::string selected_port_message_connected = "Connection success: The port " + std::string(port) + " is selected for CANopen!";
        std::string selected_port_message_not_connected = "Connection issue: The port " + std::string(port) + " cannot be connected!";
        Tools_Gui_CreateDialogs_showPopUpInformationDialogOK("selected CANopen port - connected", selected_port_message_connected.c_str());
        Tools_Gui_CreateDialogs_showPopUpInformationDialogOK("selected CANopen port - not connected", selected_port_message_not_connected.c_str());

		ImGui::End();
	}
}