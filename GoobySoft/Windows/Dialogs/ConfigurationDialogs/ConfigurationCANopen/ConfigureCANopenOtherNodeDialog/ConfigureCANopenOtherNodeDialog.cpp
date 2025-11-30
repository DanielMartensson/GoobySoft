#include "ConfigureCANopenOtherNodeDialog.h"
#include "../../../../../Tools/Tools.h"

void Windows_Dialogs_ConfigurationiDialogs_ConfigurationCANopen_ConfigureCANopenOtherNodeDialog_showConfigureDialog(bool* configureCANopenOtherNodeDialog) {
    // Display
    if (ImGui::Begin("Configuration other node", configureCANopenOtherNodeDialog, ImGuiWindowFlags_AlwaysAutoResize)) {

        /* Function selector */
        static int otherNodefunctionIndex = 0;
        static const char* otherNodefunctionNames[] = {
            "Activate Node Configuration",
            "Set Vendor ID To Node",
            "Set Product Code To Node",
            "Set Serial Number To Node",
            "Set Node ID To Node",
            "Set Baud Rate To Node",
            "Save Node ID And Baud Rate In Node",
            "Get Vendor ID From Node",
            "Get Product Code From Node",
            "Get Revision Number From Node",
            "Get Serial Number From Node",
            "Get Node ID From Node",
            "Check If This Vendor ID Exist At The CAN Network",
            "Check If This Product Code Exist At The CAN Network",
            "Check If This Revision Number Or Lower Exist At The CAN Network",
            "Check If This Revision Number Or Higher Exist At The CAN Network",
            "Check If This Serial Number Or Lower Exist At The CAN Network",
            "Check If This Serial Number Or Higher Exist At The CAN Network",
            "Check If There Are Any Non Configured Nodes At The CAN Network",
            "Start Remote Node",
            "Stop Remote Node",
            "No NMT Stop At Communication Error Behaviour EMCY"
        };

        ImGui::PushItemWidth(1000);
        ImGui::Combo("Other node:", &otherNodefunctionIndex, otherNodefunctionNames, IM_ARRAYSIZE(otherNodefunctionNames));

        /* Get CANopen structure */
        CANopen* canopen = Tools_Hardware_ParameterStore_getEasyCANopen();

        /* Common reusable input variables */
        static uint8_t node_ID = 1;
        static uint32_t vendor_ID = 0;
        static uint32_t product_code = 0;
        static uint32_t serial_number = 0;
        static uint32_t revision_number = 0;
        static uint8_t baudrate_table_index = 0;
        static uint16_t delay = 0;

        /* Dynamic UI */
        ImGui::PushItemWidth(100);
        switch (otherNodefunctionIndex) {
            case 0: {
                static bool active = false;
                ImGui::Checkbox("Activate", &active);
                ImGui::SameLine();
                if (ImGui::Button("Set")) {
                    Easy_CANopen_Other_Node_Activate_Node_Configuration(active);
                }
            } break;

            case 1:
                ImGui::InputScalar("Vendor ID", ImGuiDataType_U32, &vendor_ID);
                ImGui::SameLine();
                if (ImGui::Button("Set")) {
                    Easy_CANopen_Other_Node_Set_Vendor_ID_To_Node(canopen, vendor_ID);
                }
                break;

            case 2:
                ImGui::InputScalar("Product code", ImGuiDataType_U32, &product_code);
                ImGui::SameLine();
                if (ImGui::Button("Set")) {
                    Easy_CANopen_Other_Node_Set_Product_Code_To_Node(canopen, product_code);
                }
                break;

            case 3:
                ImGui::InputScalar("Serial number", ImGuiDataType_U32, &serial_number);
                ImGui::SameLine();
                if (ImGui::Button("Set")) {
                    Easy_CANopen_Other_Node_Set_Serial_Number_To_Node(canopen, serial_number);
                }
                break;

            case 4:
                ImGui::InputScalar("Node ID", ImGuiDataType_U8, &node_ID);
                node_ID = node_ID <= 127 ? node_ID : 127;
                ImGui::SameLine();
                if (ImGui::Button("Set")) {
                    Easy_CANopen_Other_Node_Set_Node_ID_To_Node(canopen, node_ID);
                }
                break;

            case 5:
                ImGui::InputScalar("Baudrate table index", ImGuiDataType_U8, &baudrate_table_index);
                ImGui::SameLine();
                ImGui::InputScalar("Delay (ms)", ImGuiDataType_U16, &delay);
                ImGui::SameLine();
                if (ImGui::Button("Set")) {
                    Easy_CANopen_Other_Node_Set_Baud_Rate_To_Node(canopen, baudrate_table_index, delay);
                }
                break;

            case 6:
                if (ImGui::Button("Save")) {
                    Easy_CANopen_Other_Node_Save_Node_ID_And_Baud_Rate_In_Node(canopen);
                }
                break;

            case 7:
                ImGui::InputScalar("Vendor ID", ImGuiDataType_U32, &canopen->master.lss.vendor_ID, NULL, NULL, "%u", ImGuiInputTextFlags_ReadOnly);
                ImGui::SameLine();
                if (ImGui::Button("Get")) {
                    Easy_CANopen_Other_Node_Get_Vendor_ID_From_Node(canopen);
                }
                break;

            case 8:
                ImGui::InputScalar("Product Code", ImGuiDataType_U32, &canopen->master.lss.product_code, NULL, NULL, "%u", ImGuiInputTextFlags_ReadOnly);
                ImGui::SameLine();
                if (ImGui::Button("Get")) {
                    Easy_CANopen_Other_Node_Get_Product_Code_From_Node(canopen);
                }
                break;

            case 9:
                ImGui::InputScalar("Revision Number", ImGuiDataType_U32, &canopen->master.lss.revision_number, NULL, NULL, "%u", ImGuiInputTextFlags_ReadOnly);
                ImGui::SameLine();
                if (ImGui::Button("Get")) {
                    Easy_CANopen_Other_Node_Get_Revision_Number_From_Node(canopen);
                }
                break;

            case 10:
                ImGui::InputScalar("Serial Number", ImGuiDataType_U32, &canopen->master.lss.serial_number, NULL, NULL, "%u", ImGuiInputTextFlags_ReadOnly);
                ImGui::SameLine();
                if (ImGui::Button("Get")) {
                    Easy_CANopen_Other_Node_Get_Serial_Number_From_Node(canopen);
                }
                break;

            case 11:
                ImGui::InputScalar("Node ID", ImGuiDataType_U32, &canopen->master.lss.node_ID, NULL, NULL, "%u", ImGuiInputTextFlags_ReadOnly);
                ImGui::SameLine();
                if (ImGui::Button("Get")) {
                    Easy_CANopen_Other_Node_Get_Node_ID_From_Node(canopen);
                }
                break;

            case 12:
                ImGui::InputScalar("Vendor ID", ImGuiDataType_U32, &vendor_ID);
                ImGui::SameLine();
                if (ImGui::Button("Check")) {
                    Easy_CANopen_Other_Node_Check_If_This_Vendor_ID_Exist_At_The_CAN_Network(canopen, vendor_ID);
                }
                ImGui::Text("Found: %s", canopen->master.lss.identity_found ? "Yes" : "No");
                break;

            case 13:
                ImGui::InputScalar("Product Code", ImGuiDataType_U32, &product_code);
                ImGui::SameLine();
                if (ImGui::Button("Check")) {
                    Easy_CANopen_Other_Node_Check_If_This_Product_Code_Exist_At_The_CAN_Network(canopen, product_code);
                }
                ImGui::Text("Found: %s", canopen->master.lss.identity_found ? "Yes" : "No");
                break;

            case 14:
                ImGui::InputScalar("Revision Number", ImGuiDataType_U32, &revision_number);
                ImGui::SameLine();
                if (ImGui::Button("Check")) {
                    Easy_CANopen_Other_Node_Check_If_This_Revision_Number_Or_Lower_Exist_At_The_CAN_Network(canopen, revision_number);
                }
                ImGui::Text("Found: %s", canopen->master.lss.identity_found ? "Yes" : "No");
                break;

            case 15:
                ImGui::InputScalar("Revision Number", ImGuiDataType_U32, &revision_number);
                ImGui::SameLine();
                if (ImGui::Button("Check")) {
                    Easy_CANopen_Other_Node_Check_If_This_Revision_Number_Or_Higher_Exist_At_The_CAN_Network(canopen, revision_number);
                }
                ImGui::Text("Found: %s", canopen->master.lss.identity_found ? "Yes" : "No");
                break;

            case 16:
                ImGui::InputScalar("Serial Number", ImGuiDataType_U32, &serial_number);
                ImGui::SameLine();
                if (ImGui::Button("Check")) {
                    Easy_CANopen_Other_Node_Check_If_This_Serial_Number_Or_Lower_Exist_At_The_CAN_Network(canopen, serial_number);
                }
                ImGui::Text("Found: %s", canopen->master.lss.identity_found ? "Yes" : "No");
                break;

            case 17:
                ImGui::InputScalar("Serial Number", ImGuiDataType_U32, &serial_number);
                ImGui::SameLine();
                if (ImGui::Button("Check")) {
                    Easy_CANopen_Other_Node_Check_If_This_Serial_Number_Or_Higher_Exist_At_The_CAN_Network(canopen, serial_number);
                }
                ImGui::Text("Found: %s", canopen->master.lss.identity_found ? "Yes" : "No");
                break;

            case 18:
                if (ImGui::Button("Check")) {
                    Easy_CANopen_Other_Node_Check_If_There_Are_Any_Non_Configured_Nodes_At_The_CAN_Network(canopen);
                }
                ImGui::Text("Non-configured found: %s", canopen->master.lss.non_configured_node_ID_found ? "Yes" : "No");
                break;

            case 19:
                ImGui::InputScalar("Node ID", ImGuiDataType_U8, &node_ID);
                ImGui::SameLine();
                if (ImGui::Button("Start")) {
                    Easy_CANopen_Other_Node_Start_Remote_Node(canopen, node_ID);
                }
                break;

            case 20:
                ImGui::InputScalar("Node ID", ImGuiDataType_U8, &node_ID);
                ImGui::SameLine();
                if (ImGui::Button("Stop")) {
                    Easy_CANopen_Other_Node_Stop_Remote_Node(canopen, node_ID);
                }
                break;

            case 21:
                ImGui::InputScalar("Node ID", ImGuiDataType_U8, &node_ID);
                ImGui::SameLine();
                if (ImGui::Button("Disable EMCY NMT Stop")) {
                    Easy_CANopen_Other_Node_No_NMT_Stop_At_Communication_Error_Behaviour_EMCY(canopen, node_ID);
                }
                break;

            default:
                ImGui::Text("Function not implemented.");
                break;
        }

        ImGui::End();
    }
}
