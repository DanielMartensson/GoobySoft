#include "NetworkConnectionDialog.h"
#include "../../../../Tools/Tools.h"
#include <cstring>

char ipAddress[256];
int portID = 0;
int addressID[4] = { 0 };
std::string protocols;

bool connectionNetworkCallback() {
	return Tools_Hardware_Network_openConnection(ipAddress, portID);
}

void disconnectionNetworkCallback() {
	Tools_Hardware_Network_closeConnection(ipAddress, portID);
}

void Windows_Dialogs_ConnectionDialogs_NetworkConnectionDialog_showNetworkConnectionDialog(bool* selectNetworkAddresses) {
	// Display
	ImGui::SetNextWindowSize(ImVec2(300, 350));
	if (ImGui::Begin("Select network address", selectNetworkAddresses, ImGuiWindowFlags_NoResize)) {
		// Network protocols
		const std::vector<std::string> protocolDevices = Tools_Software_Algorithms_arrayToVector(PROTOCOL_STRING);
		Tools_Gui_CreateItems_createCombo("Protocol", protocolDevices, protocols, false);

		// Connected addresses
		std::string portStr = Tools_Hardware_Network_getConnectedIpAddresses();
		const char* ports = portStr.c_str();
		if (ImGui::BeginListBox("Connected")) {
			while (*ports) {
				ImGui::Selectable(ports, false);
				ports += std::strlen(ports) + 1;
			}
			ImGui::EndListBox();
		}

		// Input integers
		if (ImGui::InputInt("Address 1", &addressID[0])) {
			if (addressID[0] > 255) {
				addressID[0] = 255;
			}
			if (addressID[0] < 0) {
				addressID[0] = 0;
			}
		}
		if (ImGui::InputInt("Address 2", &addressID[1])) {
			if (addressID[1] > 255) {
				addressID[1] = 255;
			}
			if (addressID[1] < 0) {
				addressID[1] = 0;
			}
		}
		if (ImGui::InputInt("Address 3", &addressID[2])) {
			if (addressID[2] > 255) {
				addressID[2] = 255;
			}
			if (addressID[2] < 0) {
				addressID[2] = 0;
			}
		}
		if (ImGui::InputInt("Address 4", &addressID[3])) {
			if (addressID[3] > 255) {
				addressID[3] = 255;
			}
			if (addressID[3] < 0) {
				addressID[3] = 0;
			}
		}
		if (ImGui::InputInt("Port", &portID)) {
			if (portID > 9999) {
				portID = 9999;
			}
			if (portID < 0) {
				portID = 0;
			}
		}

		// Check if it's connected
		std::sprintf(ipAddress, "%i%i%i%i", addressID[0], addressID[1], addressID[2], addressID[3]);
		bool isConnected = Tools_Hardware_Network_isConnected(ipAddress, portID) == NETWORK_STATUS_CONNECTED;

		// Connect and disconnect buttons
		Tools_Gui_CreateItems_createConnectDisconnectButtons(
			isConnected,
			true,
			"Connect to network",
			"You are connected to network",
			"Failed to connect to network",
			connectionNetworkCallback,
			"Disconnect from network",
			"You are disconnected from network",
			disconnectionNetworkCallback
		);

		// Update the ports for the protocolDevices
		Tools_Communications_Devices_updatePorts();

		ImGui::End();
	}
}