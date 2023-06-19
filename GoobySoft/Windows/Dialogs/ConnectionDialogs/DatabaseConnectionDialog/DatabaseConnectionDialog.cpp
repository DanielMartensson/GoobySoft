#include "DatabaseConnectionDialog.h"
#include "../../../../Tools/Tools.h"

// Connection callback
bool connectionCallbackDatabase() {
	MYSQL_STATUS status = Tools_Storage_Databases_openConnection(Tools_Hardware_ParameterStore_getParameterHolder()->databaseSettings.host, Tools_Hardware_ParameterStore_getParameterHolder()->databaseSettings.addDevice, Tools_Hardware_ParameterStore_getParameterHolder()->databaseSettings.username, Tools_Hardware_ParameterStore_getParameterHolder()->databaseSettings.password, Tools_Hardware_ParameterStore_getParameterHolder()->databaseSettings.schema);
	return status == MYSQL_STATUS_CONNECTED ? true : false;
}

// Disconnection callback
void disconnectionCallbackDatabase() {
	Tools_Storage_Databases_closeConnection();
}

void deleteCallbackDatabase() {
	Tools_Storage_Databases_dropSchema(Tools_Hardware_ParameterStore_getParameterHolder()->databaseSettings.schema);
	Tools_Storage_Databases_closeConnection();
}

void Windows_Dialogs_ConnectionDialogs_DatabaseConnectionDialog_showDatabaseConnectionDialog(bool* selectDatabaseServer) {
	// Display
	ImGui::SetNextWindowSize(ImVec2(300, 230));
	if (ImGui::BeginPopupModal("Select database server", selectDatabaseServer, ImGuiWindowFlags_NoResize)) {
		// Display input fields
		ImGui::InputText("Host", Tools_Hardware_ParameterStore_getParameterHolder()->databaseSettings.host, sizeof(Tools_Hardware_ParameterStore_getParameterHolder()->databaseSettings.host), ImGuiInputTextFlags_CharsNoBlank);
		ImGui::InputText("Schema", Tools_Hardware_ParameterStore_getParameterHolder()->databaseSettings.schema, sizeof(Tools_Hardware_ParameterStore_getParameterHolder()->databaseSettings.schema), ImGuiInputTextFlags_CharsNoBlank);
		ImGui::InputInt("Port", &Tools_Hardware_ParameterStore_getParameterHolder()->databaseSettings.addDevice);
		ImGui::InputText("Username", Tools_Hardware_ParameterStore_getParameterHolder()->databaseSettings.username, sizeof(Tools_Hardware_ParameterStore_getParameterHolder()->databaseSettings.username), ImGuiInputTextFlags_CharsNoBlank);
		ImGui::InputText("Password", Tools_Hardware_ParameterStore_getParameterHolder()->databaseSettings.password, sizeof(Tools_Hardware_ParameterStore_getParameterHolder()->databaseSettings.password), ImGuiInputTextFlags_Password | ImGuiInputTextFlags_CharsNoBlank);

		bool isConnected = Tools_Storage_Databases_MySQL_isConnected() == MYSQL_STATUS_CONNECTED;
		Tools_Gui_CreateItems_createConnectDisconnectButtons(
			isConnected,
			true,
			"Connect to the database",
			"You are connected to the database",
			"Failed to connect to the database",
			connectionCallbackDatabase,
			"Disconnect from database",
			"You are disconnected from the database",
			disconnectionCallbackDatabase
		);

		// Delete schema
		const char popUpId[] = "DeleteSchema";
		if (isConnected) {
			if (ImGui::Button("Delete schema")) {
				ImGui::OpenPopup(popUpId);
			}
		}
		char question[100];
		sprintf_s(question, "Do you want to delete the schema name: %s", Tools_Hardware_ParameterStore_getParameterHolder()->databaseSettings.schema);
		Tools_Gui_CreateDialogs_showPopUpConfirmDialogOKCancleWithCheckBox(popUpId, question, "Yes, I want to delete the schema", deleteCallbackDatabase);
		ImGui::End();
	}
}