#include "ViewMeasurementDialog.h"
#include "../../../../Tools/Tools.h"

static void createPlot(const char plotTitle[], rapidcsv::Document& doc, std::vector<std::string>& columnNames, size_t columnCount, size_t from, size_t to, size_t withStep) {
	// Create plots
	if (ImPlot::BeginPlot(plotTitle)) {
		// Avoid the first column at index 0. It's the time column
		for (size_t i = 1; i < columnCount; i++) {
			// Get column name
			const char* columnName = columnNames.at(i).c_str();
			if (std::strcmp(columnName, "") == 0) {
				continue;
			}

			// Get column data
			std::vector<float> column = doc.GetColumn<float>(columnName);

			// Cut yData between from and to with step
			std::vector<float> yData;
			std::vector<float> xData;
			size_t j;
			for (j = from - 1; j < to; j += withStep) {
				yData.emplace_back(column.at(j));
				xData.emplace_back(((float)j) + 1.0f);
			}

			// Plot
			ImPlot::PlotLine(columnName, xData.data(), yData.data(), (int)yData.size());
		}
		ImPlot::EndPlot();
	}
}

void Windows_Dialogs_MeasurementDialogs_ViewMeasurementDialog_showViewMeasurementDialog(bool* viewMeasurement) {
	// Display
	if (ImGui::Begin("View measurement", viewMeasurement)) {
		// File path
		ImGui::PushItemWidth(600);
		ImGui::InputText("File path", Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName, sizeof(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName), ImGuiInputTextFlags_ReadOnly);

		// Configuration
		static size_t from = 1;
		static size_t to = 1;
		static size_t withStep = 1;
		static size_t rowCount = 0;
		static size_t columnCount = 0;
		static rapidcsv::Document doc;
		static size_t filePathNameCSVIndex = 0;
		ImGui::SameLine();
		if (ImGui::Button("Load file")) {
			// Load the CSV file
			const char* filePathName = Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName;
			filePathNameCSVIndex = std::string(filePathName).find(".csv");
			if(filePathNameCSVIndex != std::string::npos){
				doc.Load(filePathName);
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
			}else{
				ImGui::OpenPopup("No .csv selected!");
			}
		}

		// Inform the user that the user has forgotten to select the .csv file
		Tools_Gui_CreateDialogs_showPopUpInformationDialogOK("No .csv selected!", "You need to select a .csv first!");

		if (ImGui::InputInt("From", (int*)&from)) {
			if (from < 1) {
				from = 1;
			}
			if (from > rowCount) {
				from = rowCount;
			}
		}
		if (ImGui::InputInt("To (enter -1 to jump to max row)", (int*)&to)) {
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
		if (columnCount > 0 && filePathNameCSVIndex != std::string::npos) {
			// Get column names
			std::vector<std::string> columnNames = doc.GetColumnNames();

			// Create two plots
			createPlot("Measurements upper", doc, columnNames, columnCount, from, to, withStep);
			createPlot("Measurements lower", doc, columnNames, columnCount, from, to, withStep);

		}
		ImGui::End();
	}
}