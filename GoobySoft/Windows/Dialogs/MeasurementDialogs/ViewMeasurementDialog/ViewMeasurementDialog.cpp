#include "ViewMeasurementDialog.h"
#include "../../../../Tools/Tools.h"

// Data table
rapidcsv::Document doc;
size_t rowCount = 0;
size_t columnCount = 0;

static void addMeasurementPlot(const char titleId[], bool selectedColumns[], std::vector<std::string>& columnNames, size_t from, size_t to, size_t withStep) {
	if (ImPlot::BeginPlot(titleId)) {
		for (size_t i = 0; i < columnCount; i++) {
			if (selectedColumns[i]) {
				// Get column data
				std::vector<float> column = doc.GetColumn<float>(columnNames.at(i));

				// Cut yData between from and to with step
				std::vector<float> yData;
				std::vector<float> xData;
				size_t j;
				for ( j = from - 1; j < to; j += withStep) {
					yData.emplace_back(column.at(j));
					xData.emplace_back(((float)j) + 1.0f);
				}

				// Plot
				int s = yData.size();
				ImPlot::PlotLine(columnNames.at(i).c_str(), xData.data(), yData.data(), s);

			}
		}
		ImPlot::EndPlot();
	}
}

void Windows_Dialogs_MeasurementDialogs_ViewMeasurementDialog_showViewMeasurementDialog(bool* viewMeasurement) {
	// Display
	if (ImGui::Begin("View measurement", viewMeasurement)) {
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
			if (from > rowCount) {
				from = rowCount;
			}
			if (to > rowCount) {
				to = rowCount;
			}
			if (withStep > rowCount) {
				withStep = rowCount;
			}
		}

		if (ImGui::InputInt("From", (int*)&from)) {
			if (from < 1) {
				from = 1;
			}
			if (from > rowCount) {
				from = rowCount;
			}
		}
		if (ImGui::InputInt("To", (int*)&to)) {
			if (to < 1) {
				to = 1;
			}
			if (to > rowCount) {
				to = rowCount;
			}
		}
		if (ImGui::InputInt("With step", (int*)&withStep)) {
			if (withStep < 1) {
				withStep = 1;
			}
			if (withStep > rowCount) {
				withStep = rowCount;
			}
		}
		ImGui::Separator();

		// Get the column names
		if (columnCount > 0) {
			// Get column names
			std::vector<std::string> columnNames = doc.GetColumnNames();

			// How many plots
			static int numberOfPlots = 1;
			const int maxPlots = 3;
			if (ImGui::InputInt("Number of plots", &numberOfPlots)) {
				if (numberOfPlots < 1) {
					numberOfPlots = 1;
				}
				if (numberOfPlots > maxPlots) {
					numberOfPlots = maxPlots;
				}
			}

			static bool selectedColumnsPlot[maxPlots][256] = { {false} };
			char text[50];
			for (int i = 0; i < numberOfPlots; i++) {
				std::sprintf(text, "Columns for plot %i", i);
				ImGui::Text(text);
				ImGui::SameLine();
				for (size_t j = 0; j < columnCount; j++) {
					if (!columnNames.at(j).empty()) {
						ImGui::Checkbox((columnNames.at(j) + std::to_string(i)).c_str(), &selectedColumnsPlot[i][j]);
						ImGui::SameLine();
					}
				}
				ImGui::NewLine();
			}
			ImGui::Separator();
			
			// Create plots
			ImPlot::CreateContext();
			for (int i = 0; i < numberOfPlots; i++) {
				std::sprintf(text, "Plot %i", i);
				addMeasurementPlot(text, selectedColumnsPlot[i], columnNames, from, to, withStep);
			}

		}
		ImGui::End();
	}
}