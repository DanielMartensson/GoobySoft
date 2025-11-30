#include "ConfigureCANopenThisNodeDialog.h"
#include "../../../../../Tools/Tools.h"

void Windows_Dialogs_ConfigurationiDialogs_ConfigurationCANopen_ConfigureCANopenThisNodeDialog_showConfigureDialog(bool* configureCANopenThisNodeDialog) {
	// Display
	if (ImGui::Begin("Configuration this node", configureCANopenThisNodeDialog, ImGuiWindowFlags_AlwaysAutoResize)) {        
        /* UI selection */
        static int thisNodefunctionIndex = 0;

        static const char* thisNodefunctionNames[] = {
            "Broadcast Bootup Message",
            "Broadcast Heartbeat Status",
            "Transmit Node Guard Status Request",
            "Configure Heartbeat Interval",
            "Configure Guard Time And Life Factor"
        };

        ImGui::PushItemWidth(600);
        ImGui::Combo("This node function:", &thisNodefunctionIndex, thisNodefunctionNames, IM_ARRAYSIZE(thisNodefunctionNames));

        /* CANopen instance */
        CANopen* canopen = Tools_Hardware_ParameterStore_getEasyCANopen();

        /* Common input variables */
        static uint8_t node_ID = 1;
        static uint16_t heartbeatInterval_ms = 1000;
        static uint16_t guardTime_ms = 100;
        static uint8_t lifeFactor = 3;

        // Dynamic input fields depending on selected function
        ImGui::PushItemWidth(600);
        switch (thisNodefunctionIndex) {
            case 0: 
                ImGui::Text("Send NMT Bootup message from this node.");
                ImGui::SameLine();
                if (ImGui::Button("Send Bootup Broadcast")) {
                    Easy_CANopen_This_Node_Broadcast_Bootup_Message(canopen);
                }
                break;

            case 1: 
                ImGui::Text("Send this node's current heartbeat status.");
                ImGui::SameLine();
                if (ImGui::Button("Send Heartbeat Status")) {
                    Easy_CANopen_This_Node_Broadcast_Heartbeat_Status(canopen);
                }
                break;

            case 2:
                ImGui::InputScalar("Target Node ID", ImGuiDataType_U8, &node_ID);
                ImGui::SameLine();
                if (ImGui::Button("Request Guard Status")) {
                    Easy_CANopen_This_Node_Transmit_Guard_Status_Request(canopen, node_ID);
                }
                break;

            case 3:
                ImGui::InputScalar("Heartbeat Interval (ms)", ImGuiDataType_U16, &heartbeatInterval_ms);
                ImGui::SameLine();
                if (ImGui::Button("Configure Heartbeat Interval")) {
                    Easy_CANopen_This_Node_Configure_Heartbeat_Interval(canopen, heartbeatInterval_ms);
                }
                break;

            case 4: // Guard time + life factor
                ImGui::InputScalar("Guard Time (ms)", ImGuiDataType_U16, &guardTime_ms);
                ImGui::SameLine();
                ImGui::InputScalar("Life Factor", ImGuiDataType_U8, &lifeFactor);
                ImGui::SameLine();
                if (ImGui::Button("Configure Guard Parameters")) {
                    Easy_CANopen_This_Node_Configure_Guard_Time_And_Life_Factor(canopen, guardTime_ms, lifeFactor);
                }
                break;

            default:
                ImGui::Text("Function not implemented.");
                break;
        }

		ImGui::End();
	}
}