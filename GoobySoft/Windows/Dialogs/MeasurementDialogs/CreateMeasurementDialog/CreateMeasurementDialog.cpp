#include "CreateMeasurementDialog.h"
#include "../../../../Tools/Tools.h"
#include <chrono>
#include <fstream>
#include <cstdio>

// File
static std::ofstream measurementFile;

// Vectors for measurements 
static std::vector<std::vector<float>> xDataADL400;
static std::vector<std::vector<float>> yDataADL400;
static std::vector<std::vector<float>> xDataSTM32PLC;
static std::vector<std::vector<float>> yDataSTM32PLC;
static std::vector<std::vector<float>> xDataOpenSAEJ1939;
static std::vector<std::vector<float>> yDataOpenSAEJ1939;

static void updateVectors(const int samples, const float value, const size_t i, std::vector<std::vector<float>>& xData, std::vector<std::vector<float>>& yData) {
	size_t s = xData.at(i).size();
	if (s > samples) {
		// Remove diff elements
		size_t diff = s - samples;
		auto xDataBegin = xData.at(i).begin();
		auto yDataBegin = yData.at(i).begin();
		xData.at(i).erase(xDataBegin, xDataBegin + diff);
		yData.at(i).erase(yDataBegin, yDataBegin + diff);

		// Add
		xData.at(i).emplace_back(xData.at(i).back() + 1); 
		yData.at(i).emplace_back(value);
	}
	else if (s == 0) {
		xData.at(i).emplace_back(0.0f);
		yData.at(i).emplace_back(value);
	}
	else {
		xData.at(i).emplace_back(xData.at(i).back() + 1); // Add +1 for each new element
		yData.at(i).emplace_back(value);
	}
}

static bool checkIfItsTimeForStoreMeasurement(const int sampleTime) {
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

static void clearAndResize(const size_t tableSize, std::vector<std::vector<float>>& xData, std::vector<std::vector<float>>& yData) {
	// Clear the vectors
	xData.clear();
	yData.clear();

	// Resize the vectors
	xData.resize(tableSize);
	yData.resize(tableSize);
}

static void plotVectors(const std::string& displayName, const size_t i, std::vector<std::vector<float>>& xData, std::vector<std::vector<float>>& yData) {
	size_t s = yData.at(i).size();
	if (s > 0) {
		ImPlot::PlotLine(displayName.c_str(), xData.at(i).data(), yData.at(i).data(), (int)s);
	}
}

static void writeHeaderForCSVFile(std::vector<std::string> displayNames) {
	const size_t displayNamesSize = displayNames.size();
	for (size_t i = 0; i < displayNamesSize; i++) {
		measurementFile << displayNames.at(i) << ",";
	}
}

static void writeMeasurement(const size_t tableSize, bool (*isInput)(size_t), float (*getMeasurementVariable)(size_t), int* (*getControlVariable)(size_t), float (*setControlVariable)(int, size_t), std::string(*getDisplayName)(size_t), bool performMeasurementUpdate, int samples, std::vector<std::vector<float>>& xData, std::vector<std::vector<float>>& yData){
	for (size_t i = 0; i < tableSize; i++) {
		// Data
		std::string displayName = getDisplayName(i);
		float value = 0.0f;
		if (isInput(i)) {
			value = getMeasurementVariable(i); // Just read the variable
		}
		else {
			value = setControlVariable(*getControlVariable(i), i); // Then read it
		}

		// Update
		if (performMeasurementUpdate) {
			updateVectors(samples, value, i, xData, yData);
		}

		// Plot
		plotVectors(displayName, i, xData, yData);

		// Add ,
		if (performMeasurementUpdate) {
			measurementFile << value << ",";
		}
	}
}

void addSlidersForControl(const size_t tableSize, bool (*isInput)(size_t), int* (*getControlVariable)(size_t), std::string(*getDisplayName)(size_t)) {
	for (size_t i = 0; i < tableSize; i++) {
		std::string displayName = getDisplayName(i);
		if (!isInput(i)) {
			ImGui::SliderInt(displayName.c_str(), getControlVariable(i), -100, 100); // Change the variable by getting access to it
		}
	}
}

void Windows_Dialogs_MeasurementDialogs_CreateMeasurementDialog_showCreateMeasurementDialog(bool* createMeasurement) {
	// Display
	if (ImGui::BeginPopupModal("Create measurement", createMeasurement)) {
		// Get the data sizes of the tables
		const size_t configurationTableDataSizeADL400 = Tools_Communications_Devices_ADL400_getConfigurationTableDataSize();
		const size_t configurationTableDataSizeSTM32PLC = Tools_Communications_Devices_STM32PLC_getConfigurationTableDataSize();
		const size_t configurationTableDataSizeOpenSAEJ1939 = Tools_Communications_Devices_OpenSAEJ1939_getConfigurationTableDataSize();
		// Add more...

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
			clearAndResize(configurationTableDataSizeSTM32PLC, xDataSTM32PLC, yDataSTM32PLC);
			clearAndResize(configurationTableDataSizeOpenSAEJ1939, xDataOpenSAEJ1939, yDataOpenSAEJ1939);
			// Add more...

			measurementFile.open(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName);
			isStarted = measurementFile.is_open();
			if (!isStarted) {
				ImGui::OpenPopup("couldNotStart");
			}
			else {
				// Write the header - Begin with time
				measurementFile << "Time,";

				// Add display names
				writeHeaderForCSVFile(Tools_Communications_Devices_ADL400_getDisplayNames());
				writeHeaderForCSVFile(Tools_Communications_Devices_STM32PLC_getDisplayNames());
				writeHeaderForCSVFile(Tools_Communications_Devices_OpenSAEJ1939_getDisplayNames());
				// Add more...

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
		std::sprintf(text, "Row %i of %i", rowCount, INT32_MAX);
		ImGui::Text(text);

		// This is only for the design
		ImGui::Separator();

		// Check if it's time for store a measurement
		bool performMeasurementUpdate = checkIfItsTimeForStoreMeasurement(sampleTime) && !pause;

		// Add the sliders
		addSlidersForControl(configurationTableDataSizeADL400, Tools_Communications_Devices_ADL400_isInput, Tools_Communications_Devices_ADL400_getControlVariable, Tools_Communications_Devices_ADL400_getDisplayName);
		addSlidersForControl(configurationTableDataSizeSTM32PLC, Tools_Communications_Devices_STM32PLC_isInput, Tools_Communications_Devices_STM32PLC_getControlVariable, Tools_Communications_Devices_STM32PLC_getDisplayName);
		addSlidersForControl(configurationTableDataSizeOpenSAEJ1939, Tools_Communications_Devices_OpenSAEJ1939_isInput, Tools_Communications_Devices_OpenSAEJ1939_getControlVariable, Tools_Communications_Devices_OpenSAEJ1939_getDisplayName);
		// Add more...

		// Start to measure
		if (isStarted) {
			// Create plot
			ImPlot::CreateContext();
			if (ImPlot::BeginPlot("Measurement")) {
				// Write time
				if (performMeasurementUpdate) {
					measurementFile << Tools_Software_Algorithms_getISO8601Time() << ",";
				}

				// Plot the measurements
				writeMeasurement(configurationTableDataSizeADL400, Tools_Communications_Devices_ADL400_isInput, Tools_Communications_Devices_ADL400_getMeasurementVariable, Tools_Communications_Devices_ADL400_getControlVariable, Tools_Communications_Devices_ADL400_setControlVariable, Tools_Communications_Devices_ADL400_getDisplayName, performMeasurementUpdate, samples, xDataADL400, yDataADL400);
				writeMeasurement(configurationTableDataSizeSTM32PLC, Tools_Communications_Devices_STM32PLC_isInput, Tools_Communications_Devices_STM32PLC_getMeasurementVariable, Tools_Communications_Devices_STM32PLC_getControlVariable, Tools_Communications_Devices_STM32PLC_setControlVariable, Tools_Communications_Devices_STM32PLC_getDisplayName, performMeasurementUpdate, samples, xDataSTM32PLC, yDataSTM32PLC);
				writeMeasurement(configurationTableDataSizeOpenSAEJ1939, Tools_Communications_Devices_OpenSAEJ1939_isInput, Tools_Communications_Devices_OpenSAEJ1939_getMeasurementVariable, Tools_Communications_Devices_OpenSAEJ1939_getControlVariable, Tools_Communications_Devices_OpenSAEJ1939_setControlVariable, Tools_Communications_Devices_OpenSAEJ1939_getDisplayName, performMeasurementUpdate, samples, xDataOpenSAEJ1939, yDataOpenSAEJ1939);
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
		ImGui::EndPopup();
	}
}
