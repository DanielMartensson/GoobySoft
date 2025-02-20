#include "ConfigurePWMDialog.h"
#include "../../../../../Tools/Tools.h"

void Windows_Dialogs_ConfigurationDialogs_ConfigurationSTM32PLC_ConfigurePWMDialog_showConfigurePWMDialog(bool* configurePWM) {
	// Display
	ImGui::SetNextWindowSize(ImVec2(350, 125));
	if (ImGui::Begin("Configure PWM", configurePWM, ImGuiWindowFlags_NoResize)) {
		// Get the CDC ports
		std::string connectedCDCports = Tools_Hardware_USB_getConnectedPorts();

		// Create combo box
		static int cdcIndex = 0;
		ImGui::PushItemWidth(100);
		ImGui::Combo("Connected CDC ports", &cdcIndex, connectedCDCports.c_str());
		char port[20];
		Tools_Software_Algorithms_extractElementFromCharArray(connectedCDCports.c_str(), cdcIndex, port);

		// Parameters for PWM
		static int prescaler_PWM0_to_PWM3 = 0;
		static int prescaler_PWM4_to_PWM7 = 0;

		// Create buttons
		if (ImGui::Button("Receive PWM prescalers")) {
			uint8_t data[4] = { STM32PLC_SEND_BACK_PWM_PRESCALERS_MESSAGE_TYPE };
			Tools_Hardware_USB_write(port, data, 1, 0);
			const int32_t result = Tools_Hardware_USB_read(port, data, 4, 100);
			if (result > 0) {
				prescaler_PWM0_to_PWM3 = (data[0] << 8) | data[1];
				prescaler_PWM4_to_PWM7 = (data[2] << 8) | data[3];
				Tools_Gui_CreateDialogs_showPopUpInformationDialogOK("Presecalers", "Presecalers received");
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Transmit PWM prescalers ")) {
			uint8_t data[5] = { STM32PLC_WRITE_SET_PWM_PRESCALER_MESSAGE_TYPE, prescaler_PWM0_to_PWM3 >> 8, prescaler_PWM0_to_PWM3 & 0xFF, prescaler_PWM4_to_PWM7 >> 8, prescaler_PWM4_to_PWM7 & 0xFF};
			Tools_Hardware_USB_write(port, data, 5, 0);
			const int32_t result = Tools_Hardware_USB_read(port, data, 1, 100);
			if (result > 0) {
				Tools_Gui_CreateDialogs_showPopUpInformationDialogOK("Presecalers", "Presecalers transmitted");
			}
		}

		// Show prescalers
		if (ImGui::InputInt("Prescaler PMW0 to PWM3", &prescaler_PWM0_to_PWM3)) {
			if (prescaler_PWM0_to_PWM3 > 0xFFFF) {
				prescaler_PWM0_to_PWM3 = 0xFFFF;
			}
			if (prescaler_PWM0_to_PWM3 < 0x0) {
				prescaler_PWM0_to_PWM3 = 0x0;
			}
		}
		if (ImGui::InputInt("Prescaler PMW4 to PWM7", &prescaler_PWM4_to_PWM7)) {
			if (prescaler_PWM4_to_PWM7 > 0xFFFF) {
				prescaler_PWM4_to_PWM7 = 0xFFFF;
			}
			if (prescaler_PWM4_to_PWM7 < 0x0) {
				prescaler_PWM4_to_PWM7 = 0x0;
			}
		}

		// End
		ImGui::End();
	}
}