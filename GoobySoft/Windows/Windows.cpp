#include "../Tools/Tools.h"
#include "Dialogs/Dialogs.h"

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

void showMainWindow(bool* done) {
	// Show the main window
	ImGui::SetNextWindowSize(ImVec2(500.0f, 350.0f));
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Select a file", nullptr, nullptr)) {
				selectFile = true;
			}
			if (ImGui::MenuItem("Create a file", nullptr, nullptr)) {
				createFile = true;
			}
			if (ImGui::MenuItem("Delete a file", nullptr, nullptr)) {
				createFile = true;
			}
			if (ImGui::MenuItem("Quit", nullptr, nullptr)) {
				*done = true; // Close the application
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Connection")) {
			if (ImGui::MenuItem("Select USB ports", nullptr, nullptr)) {
				selectUSBPorts = true;
			}
			if (ImGui::MenuItem("Select database server", nullptr, nullptr)) {
				selectDatabaseServer = true;
			}
			if (ImGui::MenuItem("Select network addresses", nullptr, nullptr)) {
				selectNetworkAddreses = true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Measurement")) {
			if (ImGui::MenuItem("Configure measurement", nullptr, nullptr)) {
				configureMeasurement = true;
			}
			if (ImGui::MenuItem("Create measurement", nullptr, nullptr)) {
				createMeasurement = true;
			}
			if (ImGui::MenuItem("View measurement", nullptr, nullptr)) {
				viewMeasurement = true;
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
			ImGui::OpenPopup("Create measurement");
			Windows_Dialogs_MeasurementDialogs_CreateMeasurementDialog_showCreateMeasurementDialog(&createMeasurement);
		}
		if (viewMeasurement) {
			Windows_Dialogs_MeasurementDialogs_ViewMeasurementDialog_showViewMeasurementDialog(&viewMeasurement);
		}

		ImGui::EndMainMenuBar();
	}
}