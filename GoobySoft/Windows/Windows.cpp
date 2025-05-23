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
bool configureAnalogInputSTM32PLC = false;
bool configureParametersWellerToJBC = false;
bool configureDateTimeSTM32PLC = false;
bool configurePWMSTM32PLC = false;
bool canTrafficDialog = false;
bool fisherFacesDialog = false;
bool landmarkDialog = false;
bool cameraDialog = false;
bool viewGradesDialog = false;


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
				ImGui::MenuItem("Analog input", nullptr, &configureAnalogInputSTM32PLC);
				ImGui::MenuItem("Date time", nullptr, &configureDateTimeSTM32PLC);
				ImGui::MenuItem("PWM", nullptr, &configurePWMSTM32PLC);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Weller To JBC")) {
				ImGui::MenuItem("Parameters", nullptr, &configureParametersWellerToJBC);
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
		if (ImGui::BeginMenu("Utils")) {
			if (ImGui::BeginMenu("Image")) {
				if (ImGui::BeginMenu("Fisherfaces")) {
					ImGui::MenuItem("Build Fisherfaces model", nullptr, &fisherFacesDialog);
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Object detection")) {
					ImGui::MenuItem("Create landmark model", nullptr, &landmarkDialog);
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Camera")) {
					ImGui::MenuItem("Open camera", nullptr, &cameraDialog);
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Database")) {
				if (ImGui::BeginMenu("Grades dialog")) {
					ImGui::MenuItem("View grades", nullptr, &viewGradesDialog);
					ImGui::EndMenu();
				}
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
			Windows_Dialogs_ConfigurationDialogs_ConfigurationSAEJ1939_ConfigureIdentificationsDialog_showConfigureIdentificationsDialog(&configureSAEJ1939Identifications);
		}
		if (configureSAEJ1939DiagnosticMessage) {
			Windows_Dialogs_ConfigurationiDialogs_ConfigurationSAEJ1939_ConfigureDiagnosticMessageDialog_showConfigureDiagnosticMessageDialog(&configureSAEJ1939DiagnosticMessage);
		}
		if (configureSAEJ1939Name) {
			Windows_Dialogs_ConfigurationiDialogs_ConfigurationSAEJ1939_ConfigureNameDialog_showConfigureNameDialog(&configureSAEJ1939Name);
		}
		if (configureAnalogInputSTM32PLC) {
			Windows_Dialogs_ConfigurationDialogs_ConfigurationSTM32PLC_ConfigureAnalogInputDialog_showConfigureAnalogInputDialog(&configureAnalogInputSTM32PLC);
		}
		if (configureParametersWellerToJBC) {
			Windows_Dialogs_ConfigurationDialogs_ConfigurationWellerToJBC_ConfigureParametersDialog_showConfigureParametersDialog(&configureParametersWellerToJBC);
		}
		if (configureDateTimeSTM32PLC) {
			Windows_Dialogs_ConfigurationDialogs_ConfigurationSTM32PLC_ConfigureDateTimeDialog_showConfigureDateTimeDialog(&configureDateTimeSTM32PLC);
		}
		if (configurePWMSTM32PLC) {
			Windows_Dialogs_ConfigurationDialogs_ConfigurationSTM32PLC_ConfigurePWMDialog_showConfigurePWMDialog(&configurePWMSTM32PLC);
		}
		if (canTrafficDialog) {
			Windows_Dialogs_AnalyzeDialogs_CANTrafficDialog_showCANTrafficDialog(&canTrafficDialog);
		}
		if (fisherFacesDialog) {
			Windows_Dialogs_UtilDialogs_ImageDialogs_FisherFacesDialog_showFisherFacesDialog(&fisherFacesDialog);
		}
		if (landmarkDialog) {
			Windows_Dialogs_UtilDialogs_ImageDialogs_LandmarkDialog_showLandmarkDialog(&landmarkDialog);
		}
		if (cameraDialog) {
			Windows_Dialogs_UtilDialogs_ImageDialogs_CameraDialog_showCameraDialog(&cameraDialog);
		}
		if (viewGradesDialog) {
			Windows_Dialogs_UtilDialogs_DatabaseDialogs_GradesDialog_showViewGradesDialog(&viewGradesDialog);
		}

		ImGui::EndMainMenuBar();
	}
}