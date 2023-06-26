#include "ViewMeasurementDialog.h"
#include "../../../../Tools/Tools.h"

// Data table
rapidcsv::Document doc;
size_t rowCount;
size_t columnCount;

void Windows_Dialogs_MeasurementDialogs_ViewMeasurementDialog_showViewMeasurementDialog(bool* viewMeasurement) {
	// Display
	if (ImGui::BeginPopupModal("View measurement", viewMeasurement)) {
		// File path
		ImGui::InputText("File path", Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName, sizeof(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName), ImGuiInputTextFlags_ReadOnly);

		// Configuration
		static size_t from = 1;
		static size_t to = 1;
		static size_t withStep = 1;
		if (ImGui::Button("Load file")) {
			// Load the CSV file
			doc.Load(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName);
			columnCount = doc.GetColumnCount();
			rowCount = doc.GetRowCount();

			// Check
			if (from >= rowCount) {
				from = rowCount;
			}
			if (to >= rowCount) {
				to = rowCount;
			}
			if (withStep >= rowCount) {
				withStep = rowCount;
			}
		}

		if (ImGui::InputInt("From", (int*)&from)) {
			if (from < 1) {
				from = 1;
			}
			if (from >= rowCount) {
				from = rowCount;
			}
		}
		if (ImGui::InputInt("To", (int*)&to)) {
			if (to < 1) {
				to = 1;
			}
			if (to >= rowCount) {
				to = rowCount;
			}
		}
		if (ImGui::InputInt("With step", (int*)&withStep)) {
			if (withStep < 1) {
				withStep = 1;
			}
			if (withStep >= rowCount) {
				withStep = rowCount;
			}
		}
		ImGui::Separator();

		// Get the column names
		if (columnCount > 0) {
			std::vector<std::string> columnNames = doc.GetColumnNames();
			static bool selectedColumns[256] = { false };
			ImGui::Text("Columns:");
			ImGui::SameLine();
			for (size_t i = 0; i < columnCount; i++) {
				if (columnNames.at(i) != "") {
					ImGui::Checkbox(columnNames.at(i).c_str(), &selectedColumns[i]);
					ImGui::SameLine();
				}
			}
			ImGui::Separator();

			// Create plot
			ImPlot::CreateContext();
			if (ImPlot::BeginPlot("Measurements")) {
				for (size_t i = 0; i < columnCount; i++) {
					if (selectedColumns[i]) {
						// Get column data
						std::vector<float> column = doc.GetColumn<float>(columnNames.at(i).c_str());

						// Cut yData between from and to with step
						std::vector<float> yData;
						std::vector<float> xData;
						for (size_t j = from - 1; j < to; j += withStep) {
							yData.push_back(column.at(j));
							xData.push_back(((float)j) + 1.0f);
						}

						// Plot
						ImPlot::PlotLine(columnNames.at(i).c_str(), xData.data(), yData.data(), (int)yData.size());

					}
				}
				ImPlot::EndPlot();
			}
		}
		ImGui::EndPopup();
	}
}