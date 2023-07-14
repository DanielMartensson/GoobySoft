#include "CreateMeasurementDialog.h"
#include "../../../../Tools/Tools.h"
#include <chrono>
#include <fstream>
#include <cstdio>

// File
static std::ofstream measurementFile;

static void createPlot(const char plotTitle[], Protocol* protocols, bool performMeasurementUpdate, int rowCount, int samplesShown, int showSamples) {
	// Create plot
	ImPlot::CreateContext();
	if (ImPlot::BeginPlot(plotTitle)) {
		// Iterate the protocols
		for (int i = 0; i < MAX_PROTOCOLS; i++) {
			// Check if the protocols is available
			if (protocols[i].isProtocolUsed) {
				// Iterate the devices
				Device* devices = protocols[i].devices;
				int deviceCount = protocols[i].deviceCount;
				for (int j = 0; j < deviceCount; j++) {
					// Iterate the rows
					TableRow* tableRows = devices[j].tableRows;
					int tableRowCount = devices[j].tableRowCount;
					for (int k = 0; k < tableRowCount; k++) {
						// Iterate the columns to find the measurement
						TableColumn* tableColumns = tableRows[k].tableColumns;
						int tableColumnCount = tableRows[k].tableColumnCount;
						float measurement;
						COLUMN_TYPE columnType;
						for (int l = 0; l < tableColumnCount; l++) {
							// Find the function
							if (tableColumns[l].tableColumnID.columnDefinition == COLUMN_DEFINITION_FUNCTION) {
								// Check the function and get the address. If no address is available, then address is NULL
								char* port = (char*)Tools_Hardware_ParameterStore_readCellvalueAtColumnDefinition(tableColumns, tableColumnCount, COLUMN_DEFINITION_PORT, &columnType);
								int address = *(int*)Tools_Hardware_ParameterStore_readCellvalueAtColumnDefinition(tableColumns, tableColumnCount, COLUMN_DEFINITION_ADDRESS, &columnType);
								
								// Do measurement now
								if (tableColumns[l].tableColumnID.columnFunction == COLUMN_FUNCTION_INPUT_SENSOR_NO_CALIBRATION) {
									measurement = tableRows[k].getInput(port, tableColumns[l].functionValueIndex, address);
								}
								if (tableColumns[l].tableColumnID.columnFunction == COLUMN_FUNCTION_INPUT_SENSOR_ADDRESS) {
									measurement = tableRows[k].getInput(port, tableColumns[l].functionValueIndex, address);
								}
								if (tableColumns[l].tableColumnID.columnFunction == COLUMN_FUNCTION_INPUT_SENSOR_ANALOG) {
									float valueMin = *(float*)Tools_Hardware_ParameterStore_readCellvalueAtColumnDefinition(tableColumns, tableColumnCount, COLUMN_DEFINITION_MIN_VALUE_REAL, &columnType);
									float valueMax = *(float*)Tools_Hardware_ParameterStore_readCellvalueAtColumnDefinition(tableColumns, tableColumnCount, COLUMN_DEFINITION_MAX_VALUE_REAL, &columnType);
									int valueMinRaw = *(int*)Tools_Hardware_ParameterStore_readCellvalueAtColumnDefinition(tableColumns, tableColumnCount, COLUMN_DEFINITION_MIN_VALUE_RAW, &columnType);
									int valueMaxRaw = *(int*)Tools_Hardware_ParameterStore_readCellvalueAtColumnDefinition(tableColumns, tableColumnCount, COLUMN_DEFINITION_MAX_VALUE_RAW, &columnType);
									measurement = Tools_Software_Algorithms_calibration(valueMinRaw, valueMaxRaw, valueMin, valueMax, tableRows[k].getInput(port, tableColumns[l].functionValueIndex, address));
								}
								if (tableColumns[l].tableColumnID.columnFunction == COLUMN_FUNCTION_OUTPUT_ACTUATOR) {
									measurement = tableRows[k].sliderValue;
								}
								break;
							}
						}

						// Update data vector
						if (performMeasurementUpdate) {
							// Shift
							if (samplesShown > showSamples) {
								for (int m = 0; m < samplesShown; m++) {
									tableRows[k].xData[m] = tableRows[k].xData[m + 1];
									tableRows[k].yData[m] = tableRows[k].yData[m + 1];
								}
								// Add at the end
								tableRows[k].xData[showSamples] = rowCount;
								tableRows[k].yData[showSamples] = measurement;
							}
							else {
								// Add at the end
								tableRows[k].xData[samplesShown] = rowCount;
								tableRows[k].yData[samplesShown] = measurement;
							}

						}

						// Plot data vector
						char* displayName = (char*)Tools_Hardware_ParameterStore_readCellvalueAtColumnDefinition(tableColumns, tableColumnCount, COLUMN_DEFINITION_DISPLAY_NAME, &columnType);
						ImPlot::PlotLine(displayName, tableRows[k].xData, tableRows[k].yData, samplesShown);

						// Add ,
						if (performMeasurementUpdate) {
							measurementFile << measurement << ",";
						}
					}
				}
			}
		}
		ImPlot::EndPlot();
	}
}

void Windows_Dialogs_MeasurementDialogs_CreateMeasurementDialog_showCreateMeasurementDialog(bool* createMeasurement) {
	// Display
	if (ImGui::BeginPopupModal("Create measurement", createMeasurement)) {
		// Get the parameter holder
		Protocol* protocols = Tools_Hardware_ParameterStore_getParameterHolder()->protocols;

		// Static fields
		static bool isStarted = false;
		static int rowCount = 0;
		static int showSamples = 1;
		static int samplesShown = 0;
		static int sampleSpeed = 1;
		// File path, Samples, Sample time
		ImGui::InputText("File path", Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName, sizeof(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName), ImGuiInputTextFlags_ReadOnly);
		ImGui::SliderInt("Show samples", &showSamples, 0, MAX_DATA_MEASUREMENT_PLOT - 1);
		ImGui::SliderInt("Sample speed [ms]", &sampleSpeed, 1, 2000);

		// Start button
		if (isStarted) {
			ImGui::PushStyleColor(ImGuiCol_Button, COLOR_GREEN);
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Button, COLOR_RED);
		}
		if (ImGui::Button("Start")) {
			// Begin first to clear the data arrays and resize them after amount of rows in the configuration map
			Tools_Communications_Devices_clear();

			// Open new file
			measurementFile.open(Tools_Hardware_ParameterStore_getParameterHolder()->fileSettings.filePathName);
			isStarted = measurementFile.is_open();
			if (isStarted) {
				// Write the header - Begin with time
				measurementFile << "Time,";

				// Write the header for the CSV file that we are going to store our data into
				for (int i = 0; i < MAX_PROTOCOLS; i++) {
					// Check if the protocols is available
					if (protocols[i].isProtocolUsed) {
						// Iterate devices
						Device* devices = protocols[i].devices;
						int deviceCount = protocols[i].deviceCount;
						for (int j = 0; j < deviceCount; j++) {
							// Iterate the rows
							TableRow* tableRows = devices[j].tableRows;
							int tableRowCount = devices[j].tableRowCount;
							for (int k = 0; k < tableRowCount; k++) {
								// Iterate the columns
								TableColumn* tableColumns = tableRows[k].tableColumns;
								int tableColumnCount = tableRows[k].tableColumnCount;
								for (int l = 0; l < tableColumnCount; l++) {
									// Check if it's display name column
									if (tableColumns[l].tableColumnID.columnDefinition == COLUMN_DEFINITION_DISPLAY_NAME) {
										// Write cell value of column
										measurementFile << tableColumns[l].cellValueString << ",";
									}
								}
							}
						}
					}
				}

				// New line
				measurementFile << std::endl;

				// Start the count
				rowCount = 0;
				samplesShown = 0;
			}
			else {
				ImGui::OpenPopup("couldNotStart");
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
		std::sprintf(text, "Row %i of %i.", rowCount, INT32_MAX);
		ImGui::SameLine();
		ImGui::Text(text);

		// Check if it's time for store a measurement
		static auto t1 = std::chrono::high_resolution_clock::now();
		auto t2 = std::chrono::high_resolution_clock::now();
		int difference = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		bool performMeasurementUpdate = false;
		static char sampleSpeedText[100];
		if (sampleSpeed <= difference) {
			performMeasurementUpdate = true && !pause;
			t1 = t2;
			std::sprintf(sampleSpeedText, "Actual sample speed[ms]: %i", difference);
		}
		ImGui::SameLine();
		ImGui::Text(sampleSpeedText);

		// This is only for the design
		ImGui::Separator();

		// Add the sliders
		for (int i = 0; i < MAX_PROTOCOLS; i++) {
			// Check if the protocols is available
			if (protocols[i].isProtocolUsed) {
				// Iterate the devices
				Device* devices = protocols[i].devices;
				int deviceCount = protocols[i].deviceCount;
				for (int j = 0; j < deviceCount; j++) {
					// Iterate the rows
					TableRow* tableRows = devices[j].tableRows;
					int tableRowCount = devices[j].tableRowCount;
					for (int k = 0; k < tableRowCount; k++) {
						// Iterate the columns
						TableColumn* tableColumns = tableRows[k].tableColumns;
						int tableColumnCount = tableRows[k].tableColumnCount;
						for (int l = 0; l < tableColumnCount; l++) {
							if (tableColumns[l].tableColumnID.columnDefinition == COLUMN_DEFINITION_FUNCTION) {								
								// Is the function an output function
								if (tableColumns[l].tableColumnID.columnFunction == COLUMN_FUNCTION_OUTPUT_ACTUATOR || tableColumns[l].tableColumnID.columnFunction == COLUMN_FUNCTION_OUTPUT_ACTUATOR_ADDRESS) {
									// Find important columns for creating a slider
									COLUMN_TYPE columnType;
									void* value = Tools_Hardware_ParameterStore_readCellvalueAtColumnDefinition(tableColumns, tableColumnCount, COLUMN_DEFINITION_DISPLAY_NAME, &columnType);
									const char* displayName = value == nullptr ? "ERROR" : (const char*)value;
									value = Tools_Hardware_ParameterStore_readCellvalueAtColumnDefinition(tableColumns, tableColumnCount, COLUMN_DEFINITION_PORT, &columnType);
									const char* port = value == nullptr ? "ERROR" : (const char*)value;
									value = Tools_Hardware_ParameterStore_readCellvalueAtColumnDefinition(tableColumns, tableColumnCount, COLUMN_DEFINITION_MIN_VALUE_RAW, &columnType);
									int valueMinRaw = value == nullptr ? -1 : *(int*)value;
									value = Tools_Hardware_ParameterStore_readCellvalueAtColumnDefinition(tableColumns, tableColumnCount, COLUMN_DEFINITION_MAX_VALUE_RAW, &columnType);
									int valueMaxRaw = value == nullptr ? -1 : *(int*)value;
									value = Tools_Hardware_ParameterStore_readCellvalueAtColumnDefinition(tableColumns, tableColumnCount, COLUMN_DEFINITION_ADDRESS, &columnType);
									int address = value == nullptr ? -1 : *(int*)value;
									int outputIndex = tableColumns[l].functionValueIndex;

									// Add slider
									ImGui::SliderInt(displayName, &tableRows[k].sliderValue, valueMinRaw, valueMaxRaw);

									// Send
									bool sendOK = tableRows[k].setOutput(port, outputIndex, address, tableRows[k].sliderValue);
									if (!sendOK) {
										tableRows[k].sliderValue = -1;
									}
								}
								break;
							}
						}
					}
				}
			}
		}

		// Start to measure
		if (isStarted) {
			// Write time
			if (performMeasurementUpdate) {
				measurementFile << Tools_Software_Algorithms_getISO8601Time() << ",";
			}

			// Create two plots of same type
			createPlot("Measurements upper", protocols, performMeasurementUpdate, rowCount, samplesShown, showSamples);
			createPlot("Measurements lower", protocols, performMeasurementUpdate, rowCount, samplesShown, showSamples);

			// New line in measurement file
			if (performMeasurementUpdate) {
				measurementFile << std::endl;
				rowCount++;

				// Threshold
				if (samplesShown > showSamples) {
					samplesShown = showSamples;
				}
				samplesShown++; // Important to have this after the threshold, due to the shift
			}
		}
		ImGui::EndPopup();
	}
}