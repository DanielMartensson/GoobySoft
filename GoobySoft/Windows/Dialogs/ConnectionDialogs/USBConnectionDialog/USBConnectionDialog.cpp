#include "USBConnectionDialog.h"
#include "../../../../Tools/Tools.h"
#include <cstring>

size_t portIndex;

bool connectionUSBCallback() {
	USBSettings usbSettings = Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex];
	return Tools_Hardware_USB_openConnection(usbSettings.port, usbSettings.baudrate, usbSettings.dataBit, USB_CONTROL_FLOW_STRING[usbSettings.controlFlowIndex], USB_STOP_BITS_STRING[usbSettings.stopBitIndex], USB_PARITY_STRING[usbSettings.parityIndex], PROTOCOL_STRING[usbSettings.protocolIndex]) == USB_STATUS_CONNECTED;
}

void disconnectionUSBCallback() {
	USBSettings usbSettings = Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex];
	Tools_Hardware_USB_closeConnection(usbSettings.port);
}

void Windows_Dialogs_ConnectionDialogs_USBConnectionDialog_showUSBConnectionDialog(bool* selectUSBPorts) {
	// Display
	ImGui::SetNextWindowSize(ImVec2(320, 320));
	if (ImGui::Begin("Select USB ports", selectUSBPorts, ImGuiWindowFlags_NoResize)) {
		// Scan protocols names
		static std::vector<std::string> ports;
		if (ImGui::Button("Scan USB ports")) {
			ports.clear();
			ports = Tools_Hardware_USB_getAllPorts();
		}

		// Static fields
		static bool isConnected = false;
		static std::string port;

		// Selected port name
		Tools_Gui_CreateItems_createCombo("USB ports", ports, port, false);

		// Find that index
		portIndex = Tools_Hardware_USB_getPortIndex(port.c_str());

		// Remember 
		USBSettings* usbSettings = &Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex];
		std::strcpy(usbSettings->port, port.c_str());

		// USB protocols
		const std::vector<std::string> protocolDevices = Tools_Software_Algorithms_arrayToVector(PROTOCOL_STRING);
		std::string protocols = PROTOCOL_STRING[usbSettings->protocolIndex];
		Tools_Gui_CreateItems_createCombo("Protocol", protocolDevices, protocols, isConnected);
		usbSettings->protocolIndex = Tools_Software_Algorithms_findIndexOf(PROTOCOL_STRING, protocols);

		// Check if it's connected
		isConnected = Tools_Hardware_USB_isConnected(port.c_str()) == USB_STATUS_CONNECTED;

		// Baudrate
		const std::vector<std::string> baudrates = { "110", "150", "300", "600", "1200", "1800", "2400", "4800", "9600", "19200", "38400", "57600", "115200" };
		std::string baudrate = std::to_string(usbSettings->baudrate);
		Tools_Gui_CreateItems_createCombo("Baudrate", baudrates, baudrate, isConnected);
		usbSettings->baudrate = (unsigned int)std::stoi(baudrate);
		if (usbSettings->baudrate < 110) {
			usbSettings->baudrate = 9600;
		}
		if (usbSettings->baudrate > 115200) {
			usbSettings->baudrate = 115200;
		}

		// Parity
		const std::vector<std::string> paraties = Tools_Software_Algorithms_arrayToVector(USB_PARITY_STRING);
		std::string parity = USB_PARITY_STRING[usbSettings->parityIndex];
		Tools_Gui_CreateItems_createCombo("Parity", paraties, parity, isConnected);
		usbSettings->parityIndex = Tools_Software_Algorithms_findIndexOf(USB_PARITY_STRING, parity);

		// Stop bits 
		const std::vector<std::string> stopBits = Tools_Software_Algorithms_arrayToVector(USB_STOP_BITS_STRING);
		std::string stopBit = USB_STOP_BITS_STRING[usbSettings->stopBitIndex];
		Tools_Gui_CreateItems_createCombo("Stop bits", stopBits, stopBit, isConnected);
		usbSettings->stopBitIndex = Tools_Software_Algorithms_findIndexOf(USB_STOP_BITS_STRING, stopBit);

		// Control flow
		const std::vector<std::string> controlFlows = Tools_Software_Algorithms_arrayToVector(USB_CONTROL_FLOW_STRING);
		std::string controlFlow = USB_CONTROL_FLOW_STRING[usbSettings->controlFlowIndex];
		Tools_Gui_CreateItems_createCombo("Control flow", controlFlows, controlFlow, isConnected);
		usbSettings->controlFlowIndex = Tools_Software_Algorithms_findIndexOf(USB_CONTROL_FLOW_STRING, controlFlow);

		// Data bits 
		const std::vector<std::string> dataBits = { "5", "6", "7", "8" };
		std::string dataBit = std::to_string(usbSettings->dataBit);
		Tools_Gui_CreateItems_createCombo("Data bits", dataBits, dataBit, isConnected);
		usbSettings->dataBit = (unsigned int)std::stoi(dataBit);
		if (usbSettings->dataBit < 5) {
			usbSettings->dataBit = 5;
		}
		if (usbSettings->dataBit > 8) {
			usbSettings->dataBit = 8;
		}

		// Connect and disconnect buttons
		Tools_Gui_CreateItems_createConnectDisconnectButtons(
			isConnected,
			ports.size() > 0,
			"Connect to USB",
			"You are connected to USB",
			"Failed to connect to USB",
			connectionUSBCallback,
			"Disconnect from USB",
			"You are disconnected from USB",
			disconnectionUSBCallback
		);

		// Update the ports for the devices
		Tools_Communications_Devices_updatePorts();

		ImGui::End();
	}
}