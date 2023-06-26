#include "USBConnectionDialog.h"
#include "../../../../Tools/Tools.h"
#include <cstring>

size_t portIndex;

bool connectionUSBCallback() {
	return Tools_Hardware_USB_openConnection(Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex].port,
		Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex].baudrate,
		Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex].dataBit,
		USB_CONTROL_FLOW_STRING[Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex].controlFlowIndex],
		USB_STOP_BITS_STRING[Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex].stopBitIndex],
		USB_PARITY_STRING[Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex].parityIndex],
		USB_PROTOCOL_STRING[Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex].protocolIndex]) == USB_STATUS_CONNECTED;
}

void disconnectionUSBCallback() {
	Tools_Hardware_USB_closeConnection(Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex].port,
		USB_PROTOCOL_STRING[Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex].protocolIndex]);
}

void Windows_Dialogs_ConnectionDialogs_USBConnectionDialog_showUSBconnectionDialog(bool *selectUSBPorts) {
	// Display
	ImGui::SetNextWindowSize(ImVec2(320, 320));
	if (ImGui::BeginPopupModal("Select USB ports", selectUSBPorts, ImGuiWindowFlags_NoResize)) {
		// Scan protocol names
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
		std::strcpy(Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex].port, port.c_str());

		// USB protocol
		const std::vector<std::string> protocols = Tools_Software_Algorithms_arrayToVector(USB_PROTOCOL_STRING);
		std::string protocol = USB_PROTOCOL_STRING[Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex].protocolIndex];
		Tools_Gui_CreateItems_createCombo("Protocol", protocols, protocol, isConnected);
		Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex].protocolIndex = Tools_Software_Algorithms_findIndexOf(USB_PROTOCOL_STRING, protocol);

		// Check if it's connected
		isConnected = Tools_Hardware_USB_isConnected(port.c_str(), protocol) == USB_STATUS_CONNECTED;

		// Baudrate
		const std::vector<std::string> baudrates = { "110", "150", "300", "600", "1200", "1800", "2400", "4800", "9600", "19200", "38400", "57600", "115200" };
		unsigned int* baudratePtr = &Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex].baudrate;
		std::string baudrate = std::to_string(*baudratePtr);
		Tools_Gui_CreateItems_createCombo("Baudrate", baudrates, baudrate, isConnected);
		*baudratePtr = (unsigned int)std::stoi(baudrate);
		if (*baudratePtr < 110) {
			*baudratePtr = 9600;
		}
		if (*baudratePtr > 115200) {
			*baudratePtr = 115200;
		}

		// Parity
		const std::vector<std::string> paraties = Tools_Software_Algorithms_arrayToVector(USB_PARITY_STRING);
		std::string parity = USB_PARITY_STRING[Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex].parityIndex];
		Tools_Gui_CreateItems_createCombo("Parity", paraties, parity, isConnected);
		Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex].parityIndex = Tools_Software_Algorithms_findIndexOf(USB_PARITY_STRING, parity);

		// Stop bits 
		const std::vector<std::string> stopBits = Tools_Software_Algorithms_arrayToVector(USB_STOP_BITS_STRING);
		std::string stopBit = USB_STOP_BITS_STRING[Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex].stopBitIndex];
		Tools_Gui_CreateItems_createCombo("Stop bits", stopBits, stopBit, isConnected);
		Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex].stopBitIndex = Tools_Software_Algorithms_findIndexOf(USB_STOP_BITS_STRING, stopBit);

		// Control flow
		const std::vector<std::string> controlFlows = Tools_Software_Algorithms_arrayToVector(USB_CONTROL_FLOW_STRING);
		std::string controlFlow = USB_CONTROL_FLOW_STRING[Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex].controlFlowIndex];
		Tools_Gui_CreateItems_createCombo("Control flow", controlFlows, controlFlow, isConnected);
		Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex].controlFlowIndex = Tools_Software_Algorithms_findIndexOf(USB_CONTROL_FLOW_STRING, controlFlow);

		// Data bits 
		const std::vector<std::string> dataBits = { "5", "6", "7", "8" };
		unsigned int* dataBitPtr = &Tools_Hardware_ParameterStore_getParameterHolder()->usbSettings[portIndex].dataBit;
		std::string dataBit = std::to_string(*dataBitPtr);
		Tools_Gui_CreateItems_createCombo("Data bits", dataBits, dataBit, isConnected);
		*dataBitPtr = (unsigned int)std::stoi(dataBit);
		if (*dataBitPtr < 5) {
			*dataBitPtr = 5;
		}
		if (*dataBitPtr > 8) {
			*dataBitPtr = 8;
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

		ImGui::End();
	}
}