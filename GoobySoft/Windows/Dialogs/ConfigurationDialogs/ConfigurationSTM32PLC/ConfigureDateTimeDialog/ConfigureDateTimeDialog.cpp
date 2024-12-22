#include "ConfigureDateTimeDialog.h"
#include "../../../../../Tools/Tools.h"

void Windows_Dialogs_ConfigurationDialogs_ConfigurationSTM32PLC_ConfigureDateTimeDialog_showConfigureDateTimeDialog(bool* configureDateTime) {
	// Display
	ImGui::SetNextWindowSize(ImVec2(360, 370));
	if (ImGui::Begin("Configure date time", configureDateTime, ImGuiWindowFlags_NoResize)) {
		// Get the CDC ports
		std::string connectedCDCports = Tools_Hardware_USB_getConnectedPorts();

		// Create combo box
		static int cdcIndex = 0;
		ImGui::PushItemWidth(100);
		ImGui::Combo("Connected CDC ports", &cdcIndex, connectedCDCports.c_str());
		char port[20];
		Tools_Software_Algorithms_extractElementFromCharArray(connectedCDCports.c_str(), cdcIndex, port);

		// Parameters for date time 
		static int year = 2000;
		static int month = 1;
		static int date = 1;
		static int week_day = 1;
		static int hours = 0;
		static int minutes = 0;
		static bool enableDateAlarm = false;
		static bool enableWeekAlarm = false;
		static bool isDateAlarmActive = false;
		static bool isWeekAlarmActive = false;

		// Create buttons
		if (ImGui::Button("Receive date time")) {
			uint8_t dataTX[1] = { STM32PLC_SEND_BACK_DATE_TIME_MESSAGE_TYPE };
			std::vector<uint8_t> dataRX = Tools_Hardware_USB_Protocols_CDC_writeThenRead(port, 1000, dataTX, sizeof(dataTX));
			if (!dataRX.empty()) {
				year = 2000 + dataRX.at(0);
				month = dataRX.at(1);
				date = dataRX.at(2);
				week_day = dataRX.at(3) + 1;
				hours = dataRX.at(4);
				minutes = dataRX.at(5);
				Tools_Gui_CreateDialogs_showPopUpInformationDialogOK("Date time", "Date time received");
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Transmit date time")) {
			uint8_t dataTX[7] = { STM32PLC_WRITE_SET_DATE_TIME_MESSAGE_TYPE, year - 2000, month, date, week_day - 1, hours, minutes };
			std::vector<uint8_t> dataRX = Tools_Hardware_USB_Protocols_CDC_writeThenRead(port, 1000, dataTX, sizeof(dataTX));
			if (!dataRX.empty()) {
				Tools_Gui_CreateDialogs_showPopUpInformationDialogOK("Date time", "Date time transmitted");
			}
		}
		if (ImGui::Button("Receive date alarm")) {
			uint8_t dataTX[1] = { STM32PLC_SEND_BACK_ALARM_A_MESSAGE_TYPE };
			std::vector<uint8_t> dataRX = Tools_Hardware_USB_Protocols_CDC_writeThenRead(port, 1000, dataTX, sizeof(dataTX));
			if (!dataRX.empty()) {
				date = dataRX.at(0);
				hours = dataRX.at(1);
				minutes = dataRX.at(2);
				enableDateAlarm = dataRX.at(3);
				isDateAlarmActive = dataRX.at(4);
				Tools_Gui_CreateDialogs_showPopUpInformationDialogOK("Date alarm", "Date alarm received");
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Transmit date alarm")) {
			uint8_t dataTX[5] = { STM32PLC_WRITE_SET_ALARM_A_MESSAGE_TYPE, date, hours, minutes, enableDateAlarm };
			std::vector<uint8_t> dataRX = Tools_Hardware_USB_Protocols_CDC_writeThenRead(port, 1000, dataTX, sizeof(dataTX));
			if (!dataRX.empty()) {
				Tools_Gui_CreateDialogs_showPopUpInformationDialogOK("Date alarm", "Date alarm transmitted");
			}
		}
		if (ImGui::Button("Receive week day alarm")) {
			uint8_t dataTX[1] = { STM32PLC_SEND_BACK_ALARM_B_MESSAGE_TYPE };
			std::vector<uint8_t> dataRX = Tools_Hardware_USB_Protocols_CDC_writeThenRead(port, 1000, dataTX, sizeof(dataTX));
			if (!dataRX.empty()) {
				week_day = dataRX.at(0) + 1;
				hours = dataRX.at(1);
				minutes = dataRX.at(2);
				enableWeekAlarm = dataRX.at(3);
				isWeekAlarmActive = dataRX.at(4);
				Tools_Gui_CreateDialogs_showPopUpInformationDialogOK("Week day alarm", "Week day alarm received");
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Transmit week day alarm")) {
			uint8_t dataTX[5] = { STM32PLC_WRITE_SET_ALARM_B_MESSAGE_TYPE, week_day - 1, hours, minutes, enableWeekAlarm };
			std::vector<uint8_t> dataRX = Tools_Hardware_USB_Protocols_CDC_writeThenRead(port, 1000, dataTX, sizeof(dataTX));
			if (!dataRX.empty()) {
				Tools_Gui_CreateDialogs_showPopUpInformationDialogOK("Week day alarm", "Week day alarm transmitted");
			}
		}

		// Create inputs
		if (ImGui::InputInt("Year", &year)) {
			if (year < 2000) {
				year = 2000;
			}
			if (year > 2099) {
				year = 2099;
			}
		}
		if (ImGui::InputInt("Month", &month)) {
			if (month > 12) {
				month = 12;
			}
			if(month < 1){
				month = 1;
			}
		}
		ImGui::Separator();
		ImGui::Text("Alarm configurations:");
		if (ImGui::InputInt("Date", &date)) {
			if (date > 31) {
				date = 31;
			}
			if (date < 1) {
				date = 1;
			}
		}
		if (ImGui::InputInt("Week day", &week_day)) {
			if (week_day > 7) {
				week_day = 7;
			}
			if (week_day < 1) {
				week_day = 1;
			}
		}
		if (ImGui::InputInt("Hour", &hours)) {
			if (hours > 23) {
				hours = 23;
			}
			if (hours < 0) {
				hours = 0;
			}
		}
		if (ImGui::InputInt("Minute", &minutes)) {
			if (minutes > 59) {
				minutes = 59;
			}
			if (minutes < 0) {
				minutes = 0;
			}
		}
		ImGui::Checkbox("Date alarm active", &enableDateAlarm);
		ImGui::Checkbox("Week alarm active", &enableWeekAlarm);

		char text[100];
		sprintf(text, "Date alarm is: %s", isDateAlarmActive ? "active(PMW4 -> PWM7 open)" : "inactive(PMW4 -> PWM7 closed)");
		ImGui::Text(text);
		sprintf(text, "Week alarm is: %s", isWeekAlarmActive ? "active(PMW0 -> PWM3 open)" : "inactive(PMW0 -> PWM3 closed)");
		ImGui::Text(text);

		// End
		ImGui::End();
	}
}