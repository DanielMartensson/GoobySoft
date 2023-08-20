#include "Windows.h"
#include "../Tools/Tools.h"

// For file dialogs
bool selectFile = false;
bool createFile = false;
bool deteteFile = false;
bool selectUSBPorts = false;
bool selectNetworkAddreses = false;
bool selectDatabaseServer = false;
bool configureMeasurement = false;
bool createMeasurement = false;
bool viewMeasurement = false;
bool configureSAEJ1939Name = false;
bool configureSAEJ1939Identifications = false;
bool configureSAEJ1939DiagnosticMessage = false;
bool configureAnalogInput = false;
bool configureDateTime = false;
bool configurePWM = false;
bool canTrafficDialog = false;

void showMainWindow(bool* done) {
	// Show the main window
	ImGui::SetNextWindowSize(ImVec2(500.0f, 350.0f));
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			ImGui::MenuItem("Select a file", nullptr, &selectFile);
			ImGui::MenuItem("Create a file", nullptr, &createFile);
			ImGui::MenuItem("Delete a file", nullptr, &createFile);
			ImGui::MenuItem("Quit", nullptr, done);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Connection")) {
			ImGui::MenuItem("Select USB ports", nullptr, &selectUSBPorts);
			ImGui::MenuItem("Select database server", nullptr, &selectDatabaseServer);
			ImGui::MenuItem("Select network addresses", nullptr, &selectNetworkAddreses);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Measurement")) {
			ImGui::MenuItem("Configure measurement", nullptr, &configureMeasurement);
			ImGui::MenuItem("Create measurement", nullptr, &createMeasurement);
			ImGui::MenuItem("View measurement", nullptr, &viewMeasurement);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Configure")) {
			if (ImGui::BeginMenu("SAE J1939")) {
				ImGui::MenuItem("Name", nullptr, &configureSAEJ1939Name);
				ImGui::MenuItem("Identifications", nullptr, &configureSAEJ1939Identifications);
				ImGui::MenuItem("Diagnostics messages", nullptr, &configureSAEJ1939DiagnosticMessage);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("STM32 PLC")) {
				ImGui::MenuItem("Analog input", nullptr, &configureAnalogInput);
				ImGui::MenuItem("Date time", nullptr, &configureDateTime);
				ImGui::MenuItem("PWM", nullptr, &configurePWM);
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Analyze")) {
			if (ImGui::BeginMenu("STM32 PLC")) {
				ImGui::MenuItem("CAN traffic", nullptr, &canTrafficDialog);
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		/* Dialogs */
		if (selectFile) {
			Windows_Dialogs_FileDialogs_LogFilePathDialog_showSelectFileDialog(&selectFile);
		}
		if (createFile) {
			Windows_Dialogs_FileDialogs_LogFilePathDialog_showCreateFileDialog(&createFile);
		}
		if (createFile) {
			Windows_Dialogs_FileDialogs_LogFilePathDialog_showDeleteFileDialog(&deteteFile);
		}
		if (selectUSBPorts) {
			Windows_Dialogs_ConnectionDialogs_USBConnectionDialog_showUSBConnectionDialog(&selectUSBPorts);
		}
		if (selectNetworkAddreses) {
			Windows_Dialogs_ConnectionDialogs_NetworkConnectionDialog_showNetworkConnectionDialog(&selectNetworkAddreses);
		}
		if (selectDatabaseServer) {
			Windows_Dialogs_ConnectionDialogs_DatabaseConnectionDialog_showDatabaseConnectionDialog(&selectDatabaseServer);
		}
		if (configureMeasurement) {
			Windows_Dialogs_MeasurementDialogs_ConfigureMeasurementDialog_showConfigureMeasurementDialog(&configureMeasurement);
		}
		if (createMeasurement) {
			Windows_Dialogs_MeasurementDialogs_CreateMeasurementDialog_showCreateMeasurementDialog(&createMeasurement);
		}
		if (viewMeasurement) {
			Windows_Dialogs_MeasurementDialogs_ViewMeasurementDialog_showViewMeasurementDialog(&viewMeasurement);
		}
		if (configureSAEJ1939Identifications) {
			Windows_Dialogs_ConfigurationiDialogs_ConfigurationSAEJ1939_ConfigureIdentificationsDialog_showConfigureIdentificationsDialog(&configureSAEJ1939Identifications);
		}
		if (configureSAEJ1939DiagnosticMessage) {
			Windows_Dialogs_ConfigurationiDialogs_ConfigurationSAEJ1939_ConfigureDiagnosticMessageDialog_showConfigureDiagnosticMessageDialog(&configureSAEJ1939DiagnosticMessage);
		}
		if (configureSAEJ1939Name) {
			Windows_Dialogs_ConfigurationiDialogs_ConfigurationSAEJ1939_ConfigureNameDialog_showConfigureNameDialog(&configureSAEJ1939Name);
		}
		if (configureAnalogInput) {
			Windows_Dialogs_ConfigurationDialogs_ConfigurationSTM32PLC_ConfigureAnalogInputDialog_showConfigureAnalogInputDialog(&configureAnalogInput);
		}
		if (configureDateTime) {
			Windows_Dialogs_ConfigurationDialogs_ConfigurationSTM32PLC_ConfigureDateTimeDialog_showConfigureDateTimeDialog(&configureDateTime);
		}
		if (configurePWM) {
			Windows_Dialogs_ConfigurationDialogs_ConfigurationSTM32PLC_ConfigurePWMDialog_showConfigurePWMDialog(&configurePWM);
		}
		if (canTrafficDialog) {
			Windows_Dialogs_AnalyzeDialogs_CANTrafficDialog_showCANTrafficDialog(&canTrafficDialog);
		}

		ImGui::EndMainMenuBar();
	}
}