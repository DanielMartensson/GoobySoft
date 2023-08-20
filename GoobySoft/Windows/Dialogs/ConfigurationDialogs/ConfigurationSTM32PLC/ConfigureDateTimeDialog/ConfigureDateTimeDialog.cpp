#include "ConfigureDateTimeDialog.h"
#include "../../../../../Tools/Tools.h"

void Windows_Dialogs_ConfigurationDialogs_ConfigurationSTM32PLC_ConfigureDateTimeDialog_showConfigureDateTimeDialog(bool* configureDateTime) {
	// Display
	ImGui::SetNextWindowSize(ImVec2(300, 370));
	if (ImGui::Begin("Configure date time", configureDateTime, ImGuiWindowFlags_NoResize)) {
		// Get the CDC ports
		std::string connectedCDCports = Tools_Hardware_USB_getConnectedPorts(USB_PROTOCOL_STRING[USB_PROTOCOL_ENUM_CDC]);

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
			std::vector<uint8_t> dataRX = Tools_Hardware_USB_Protocols_CDC_startTransceiveProcesss(port, 1000, dataTX, sizeof(dataTX));
			if (!dataRX.empty()) {
				year = 2000 + dataRX.at(1);
				month = dataRX.at(2);
				date = dataRX.at(3);
				week_day = dataRX.at(4);
				hours = dataRX.at(5);
				minutes = dataRX.at(6);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Transmit date time")) {
			uint8_t dataTX[7] = { STM32PLC_WRITE_SET_DATE_TIME_MESSAGE_TYPE, year - 2000, month, week_day, hours, minutes };
			Tools_Hardware_USB_Protocols_CDC_startTransceiveProcesss(port, 1000, dataTX, sizeof(dataTX));
		}
		if (ImGui::Button("Receive date alarm")) {
			uint8_t dataTX[1] = { STM32PLC_SEND_BACK_ALARM_A_MESSAGE_TYPE };
			std::vector<uint8_t> dataRX = Tools_Hardware_USB_Protocols_CDC_startTransceiveProcesss(port, 1000, dataTX, sizeof(dataTX));
			if (!dataRX.empty()) {
				date = dataRX.at(1);
				hours = dataRX.at(2);
				minutes = dataRX.at(3);
				enableDateAlarm = dataRX.at(4);
				isDateAlarmActive = dataRX.at(5);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Transmit date alarm")) {
			uint8_t dataTX[5] = { STM32PLC_WRITE_SET_ALARM_A_MESSAGE_TYPE, date, hours, minutes, enableDateAlarm };
			Tools_Hardware_USB_Protocols_CDC_startTransceiveProcesss(port, 1000, dataTX, sizeof(dataTX));
		}
		if (ImGui::Button("Receive week alarm")) {
			uint8_t dataTX[1] = { STM32PLC_SEND_BACK_ALARM_B_MESSAGE_TYPE };
			std::vector<uint8_t> dataRX = Tools_Hardware_USB_Protocols_CDC_startTransceiveProcesss(port, 1000, dataTX, sizeof(dataTX));
			if (!dataRX.empty()) {
				week_day = dataRX.at(1);
				hours = dataRX.at(2);
				minutes = dataRX.at(3);
				enableWeekAlarm = dataRX.at(4);
				isWeekAlarmActive = dataRX.at(5);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Transmit week alarm")) {
			uint8_t dataTX[5] = { STM32PLC_WRITE_SET_ALARM_B_MESSAGE_TYPE, week_day, hours, minutes, enableWeekAlarm };
			Tools_Hardware_USB_Protocols_CDC_startTransceiveProcesss(port, 1000, dataTX, sizeof(dataTX));
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

		char text[50];
		sprintf(text, "Date alarm is: %s", isDateAlarmActive ? "active" : "inactive");
		ImGui::Text(text);
		sprintf(text, "Week alarm is: %s", isWeekAlarmActive ? "active" : "inactive");
		ImGui::Text(text);

		// End
		ImGui::End();
	}
}