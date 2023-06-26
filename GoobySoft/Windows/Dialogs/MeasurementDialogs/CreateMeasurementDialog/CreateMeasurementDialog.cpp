#include "CreateMeasurementDialog.h"
#include "../../../../Tools/Tools.h"
#include <chrono>
#include <fstream>
#include <stdio.h>

// File
std::ofstream measurementFile;

// Vectors for measurements 
std::vector<std::vector<float>> xDataADL400;
std::vector<std::vector<float>> yDataADL400;

void updateVectors(const int samples, const float value, const size_t i, std::vector<std::vector<float>>& xData, std::vector<std::vector<float>>& yData) {
	size_t s = xData.at(i).size();
	if (s > samples) {
		// Remove diff elements
		size_t diff = s - samples;
		xData.at(i).erase(xData.at(i).begin(), xData.at(i).begin() + diff);
		yData.at(i).erase(yData.at(i).begin(), yData.at(i).begin() + diff);

		// Add
		xData.at(i).push_back(xData.at(i).back() + 1); 
		yData.at(i).push_back(value);
	}
	else if (s == 0) {
		xData.at(i).push_back(0.0f);
		yData.at(i).push_back(value);
	}
	else {
		xData.at(i).push_back(xData.at(i).back() + 1); // Add +1 for each new element
		yData.at(i).push_back(value);
	}


}

bool checkIfItsTimeForStoreMeasurement(const int sampleTime) {
	// Count the time in milliseconds
	static auto t1 = std::chrono::high_resolution_clock::now();
	auto t2 = std::chrono::high_resolution_clock::now();
	size_t difference = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	bool performVectorUpdate = false;
	if (sampleTime <= difference) {
		performVectorUpdate = true;
		t1 = t2;
	}
	return performVectorUpdate;
}

void clearAndResize(const size_t tableSize, std::vector<std::vector<float>>& xData, std::vector<std::vector<float>>& yData) {
	// Clear the vectors
	xData.clear();
	yData.clear();

	// Resize the vectors
	xData.resize(tableSize);
	yData.resize(tableSize);
}

void plotVectors(const std::string& displayName, const size_t i, std::vector<std::vector<float>>& xData, std::vector<std::vector<float>>& yData) {
	size_t s = yData.at(i).size();
	if (s > 0) {
		ImPlot::PlotLine(displayName.c_str(), xData.at(i).data(), yData.at(i).data(), (int)s);
	}
}

void Windows_Dialogs_MeasurementDialogs_CreateMeasurementDialog_showCreateMeasurementDialog(bool* createMeasurement) {
	// Display
	if (ImGui::BeginPopupModal("Create measurement", createMeasurement)) {
		// Get the data sizes of the tables
		const size_t configurationTableDataSizeADL400 = Tools_Communications_Devices_ADL400_getConfigurationTableDataSize();

		// File path, Samples, Sample time
		ImGui::InputText("File path", Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName, sizeof(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName), ImGuiInputTextFlags_ReadOnly);
		static int samples = 1;
		ImGui::SliderInt("Samples", &samples, 1, 2000);
		static int sampleTime = 1;
		ImGui::SliderInt("Sample time [ms]", &sampleTime, 1, 2000);

		// Start button
		static bool isStarted = false;
		static int rowCount = 0;
		if (isStarted) {
			ImGui::PushStyleColor(ImGuiCol_Button, COLOR_GREEN);
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Button, COLOR_RED);
		}
		if (ImGui::Button("Start")) {
			clearAndResize(configurationTableDataSizeADL400, xDataADL400, yDataADL400);
			measurementFile.open(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName);
			isStarted = measurementFile.is_open();
			if (!isStarted) {
				ImGui::OpenPopup("couldNotStart");
			}
			else {
				// Write the header - Begin with time
				measurementFile << "Time,";

				// ADL400 display names
				std::vector<std::string> displayNamesADL400 = Tools_Communications_Devices_ADL400_getDisplayNames();
				const size_t displayNamesADL400Size = displayNamesADL400.size();
				for (size_t i = 0; i < displayNamesADL400Size; i++) {
					measurementFile << displayNamesADL400.at(i) << ",";
				}
				// Add more..

				// New line
				measurementFile << std::endl;


				// Start the count
				rowCount = 0;
			}
		}
		ImGui::PopStyleColor();
		Tools_Gui_CreateDialogs_showPopUpInformationDialogOK("couldNotStart", "File could not be created. Please select a file.");

		// Stop button
		ImGui::SameLine();
		if (isStarted) {
			ImGui::PushStyleColor(ImGuiCol_Button, COLOR_RED);
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Button, COLOR_GREEN);
		}
		if (ImGui::Button("Stop") || rowCount == INT32_MAX) {
			isStarted = false;
			measurementFile.close();
		}
		ImGui::PopStyleColor();

		// Pause button
		static bool pause = false;
		if (pause) {
			ImGui::PushStyleColor(ImGuiCol_Button, COLOR_GREEN);
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Button, COLOR_RED);
		}
		ImGui::SameLine();
		if (ImGui::Button("Pause")) {
			pause = !pause;
		}
		ImGui::PopStyleColor();

		// Create row count
		char text[100];
		sprintf_s(text, "Row %i of %i", rowCount, INT32_MAX);
		ImGui::Text(text);

		// This is only for the design
		ImGui::Separator();

		// Check if it's time for store a measurement
		bool performMeasurementUpdate = checkIfItsTimeForStoreMeasurement(sampleTime) && !pause;

		// Start to measure
		if (isStarted) {
			// Create plot
			ImPlot::CreateContext();
			if (ImPlot::BeginPlot("Measurement")) {
				// Write time
				if (performMeasurementUpdate) {
					measurementFile << Tools_Software_Algorithms_getISO8601Time() << ",";
				}

				// Plot the measurements for ADL400
				for (size_t i = 0; i < configurationTableDataSizeADL400; i++) {
					// Data
					float value = Tools_Communications_Devices_ADL400_execute(i);
					std::string displayName = Tools_Communications_Devices_ADL400_getDisplayName(i);
					
					// Update
					if (performMeasurementUpdate) {
						updateVectors(samples, value, i, xDataADL400, yDataADL400);
					}

					// Plot
					plotVectors(displayName, i, xDataADL400, yDataADL400);
					
					// Newline
					if (performMeasurementUpdate) {
						measurementFile << value << ",";
					}
				}
				// Add more...

				// New line in measurement file
				if (performMeasurementUpdate) {
					measurementFile << std::endl;

					// Count row
					rowCount++;
				}

				// End plot
				ImPlot::EndPlot();
			}
		}

		// End context and window
		//ImPlot::DestroyContext();
		ImGui::EndPopup();
	}
}
