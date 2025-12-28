#include "ConfigureAnalogInputDialog.h"
#include "../../../../../Tools/Tools.h"

void Windows_Dialogs_ConfigurationDialogs_ConfigurationSTM32PLC_ConfigureAnalogInputDialog_showConfigureAnalogInputDialog(bool* configureAnalogInput) {
	// Display
	ImGui::SetNextWindowSize(ImVec2(450, 270));
	if (ImGui::Begin("Configure analog input", configureAnalogInput, ImGuiWindowFlags_NoResize)) {
		// Get the connected ports
		std::string connectedPorts = Tools_Hardware_USB_getConnectedPorts();

		// Create combo box
		static int cdcIndex = 0;
		ImGui::PushItemWidth(60);
		ImGui::Combo("Connected USB ports", &cdcIndex, connectedPorts.c_str());
		char port[20];
		Tools_Software_Algorithms_extractElementFromCharArray(connectedPorts.c_str(), cdcIndex, port);

		// Gains for 3 ADCs and each ADC have 3 configurations indexes.
		static int inputGains[3 * 3] = { 0 };

		// Create two buttons
		ImGui::SameLine();
		if (ImGui::Button("Receive gains")) {
			for (int adc = 0; adc < 3; adc++) {
				uint8_t data[3] = { STM32PLC_SEND_BACK_ANALOG_GAINS_MESSAGE_TYPE , adc };
				Tools_Hardware_USB_write(port, data, 2, 0);
				const int32_t result = Tools_Hardware_USB_read(port, data, 3, 100, true, false);
				if (result > 0) {
					for (int configurationIndex = 0; configurationIndex < 3; configurationIndex++) {
						inputGains[adc * 3 + configurationIndex] = data[configurationIndex];
					}
					Tools_Gui_CreateDialogs_showPopUpInformationDialogOK("Analog input", "Analog input gains received");
				}
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Transmit gains")) {
			for (int adc = 0; adc < 3; adc++) {
				uint8_t data[4] = { STM32PLC_WRITE_SET_ANALOG_INPUT_GAIN_MESSAGE_TYPE, adc };
				int count = 0;
				for (int configurationIndex = 0; configurationIndex < 3; configurationIndex++) {
					data[2] = configurationIndex;
					data[3] = inputGains[adc * 3 + configurationIndex];
					Tools_Hardware_USB_write(port, data, 4, 0);
					const int32_t result = Tools_Hardware_USB_read(port, data, 1, 100, true, false);
					if (result > 0) {
						count++;
					}
				}
				if (count == 3) {
					Tools_Gui_CreateDialogs_showPopUpInformationDialogOK("Analog input", "Analog input gains transmitted");
				}
			}
		}

		// Build up all gains for each input
		const char* gains[] = {"1X", "2X", "4X", "8X", "16X", "32X", "1/2X"};
		ImGui::Combo("Analog 0, Analog 1, Analog 2", &inputGains[0 * 3 + 0], gains, IM_ARRAYSIZE(gains));
		ImGui::Combo("Analog 3, Analog 4, Analog 5", &inputGains[0 * 3 + 1], gains, IM_ARRAYSIZE(gains));
		ImGui::Combo("Analog 6, Analog 7, Analog 8", &inputGains[0 * 3 + 2], gains, IM_ARRAYSIZE(gains));
		ImGui::Combo("Analog 9", &inputGains[1 * 3 + 0], gains, IM_ARRAYSIZE(gains));
		ImGui::Combo("Analog 10", &inputGains[1 * 3 + 1], gains, IM_ARRAYSIZE(gains));
		ImGui::Combo("Analog 11", &inputGains[1 * 3 + 2], gains, IM_ARRAYSIZE(gains));
		ImGui::Combo("Analog differential 0, Analog differential 1", &inputGains[2 * 3 + 0], gains, IM_ARRAYSIZE(gains));
		ImGui::Combo("Analog differential 2, Analog differential 3", &inputGains[2 * 3 + 1], gains, IM_ARRAYSIZE(gains));
		ImGui::Combo("Analog differential 4", &inputGains[2 * 3 + 2], gains, IM_ARRAYSIZE(gains));

		// End
		ImGui::End();
	}
}