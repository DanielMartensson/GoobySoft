#include "LandmarkDialog.h"
#include "../../../../../Tools/Tools.h"

#define MIN_SFAOD_PARAMETER_VALUE 0.0000001f

void Windows_Dialogs_UtilDialogs_ImageDialogs_LandmarkDialog_showLandmarkDialog(bool* landmarkDialog) {
	// Display
	if (ImGui::Begin("Landmark", landmarkDialog)) {
		// Get the parameter holder 
		ParameterHolder* parameterHolder = Tools_Hardware_ParameterStore_getParameterHolder();
		uint8_t* total_descriptors = &parameterHolder->models.landmark_model.total_descriptors;
		bool* is_descriptors_available = &parameterHolder->models.landmark_model.is_descriptors_available;
		uint32_t* id_descriptors = parameterHolder->models.landmark_model.id_descriptors;
		uint16_t* x_descriptors = parameterHolder->models.landmark_model.x_descriptors;
		uint16_t* y_descriptors = parameterHolder->models.landmark_model.y_descriptors;

		// Open folder
		static bool isOpen = false;
		static std::string folderPath;
		ImGui::InputText(" ", (char*)folderPath.c_str(), folderPath.length(), ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("Select folder")) {
			isOpen = true;
		}
		bool isPressedOK = false;
		Tools_Gui_CreateDialogs_showFolderDialog(&isOpen, &isPressedOK, folderPath);
		static int selectedRow = -1;
		if (isPressedOK) {
			selectedRow = -1;
		}

		// Scan folder
		if (folderPath.length() > 3) {
			std::vector<std::vector<std::string>> table;
			table.push_back({ "Picture names" });
			for (const auto& entry : std::filesystem::directory_iterator(folderPath.c_str())) {
				if (entry.path().extension().compare(".pgm") == 0) {
					table.push_back({ entry.path().string()});
				}
			}
			Tools_Gui_CreateItems_createTableSelectable("pictures_files_pgm", table, &selectedRow);
			if (selectedRow != -1) {
				// Use CControls PGM reader as image reader
				const char* s = table.at(selectedRow+1).at(0).c_str();
				PGM* image = imread(s);
				
				// Show selected image
				static GLuint texID;
				glDeleteTextures(1, &texID);
				glGenTextures(1, &texID);
				glBindTexture(GL_TEXTURE_2D, texID);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, image->width, image->height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, image->pixels);

				// Dipslay image
				ImGui::NewLine();
				ImGui::Columns(2, "MyColumns", true);
				ImGui::Image((ImTextureID)texID, ImVec2(image->width, image->height));

				// Get coordinates for the image
				const ImVec2 imageCoordinates = ImGui::GetItemRectMin();
				const ImVec2 mouseCoordinates = ImGui::GetMousePos();
				bool isInsideRegionX = false;
				float x = mouseCoordinates.x - imageCoordinates.x;
				if (x < 0) {
					x = 0;
					isInsideRegionX = false;
				}else if (x > image->width) {
					x = image->width;
					isInsideRegionX = false;
				}
				else {
					isInsideRegionX = true;
				}
				bool isInsideRegionY = false;
				float y = mouseCoordinates.y - imageCoordinates.y;
				if (y < 0) {
					y = 0;
					isInsideRegionY = false;
				}else if (y > image->height) {
					y = image->height;
					isInsideRegionY = false;
				}
				else {
					isInsideRegionY = true;
				}

				static float epsilon = 0;
				static uint8_t fast_threshold = 0;
				static uint8_t sobel_threshold = 0;
				static size_t min_pts = 0;
				if (*is_descriptors_available) {
					ImDrawList* draw_list = ImGui::GetWindowDrawList();
					for (uint8_t i = 0; i < *total_descriptors; i++) {
						draw_list->AddCircle(ImVec2(imageCoordinates.x + x_descriptors[i], imageCoordinates.y + y_descriptors[i]), 10.0f, ImColor(1.0f, 0.0f, 0.0f));
					}

					/* Collect landmarks  
					uint8_t max_classes;
					uint8_t descriptor_threshold = 0;
					float* C = generalizedhough_create_data(image->pixels, image->height, image->height, &max_classes, fast_threshold, sobel_threshold, FAST_METHOD_9, epsilon, min_pts);
					for (uint8_t i = 0; i < max_classes; i++) {
						draw_list->AddCircle(ImVec2(imageCoordinates.x + C[i*2], imageCoordinates.y + C[i * 2 + 1]), 5.0f, ImColor(0.0f, 1.0f, 0.0f));
					}
					free(C); */
					int N = min_pts;
					FAST_XY* xy = cornerdetection(image->pixels, epsilon, &N, fast_threshold, FAST_METHOD_9, image->height, image->width);
					for (int i = 0; i < N; i++) {
						draw_list->AddCircle(ImVec2(imageCoordinates.x + xy[i].x, imageCoordinates.y + xy[i].y), 5.0f, ImColor(0.0f, 1.0f, 0.0f));
					}
					free(xy);
				}

				// Create two buttons for clear
				ImGui::NextColumn();
				if (ImGui::Button("Clear landmarks")) {
					*total_descriptors = 0;
					*is_descriptors_available = false;
				}

				ImGui::InputFloat("epsilon", &epsilon);
				int min_pts_int = min_pts;
				ImGui::InputInt("min_pts", &min_pts_int);
				min_pts = min_pts_int;

				int sobel_threshold_int = sobel_threshold;
				ImGui::InputInt("sobel_threshold", &sobel_threshold_int);
				sobel_threshold = sobel_threshold_int;

				int fast_threshold_int = fast_threshold;
				ImGui::InputInt("fast_threshold", &fast_threshold_int);
				fast_threshold = fast_threshold_int;


				// Get mouse cursor point
				char text[100];
				sprintf(text, "x:%f, y:%f", x, y);
				ImGui::SameLine();
				ImGui::Text(text);
				
				// Check if mouse is clicked
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && isInsideRegionX && isInsideRegionY) {
					// Get a descriptor 
					uint32_t descriptor = landmarkdetection_analyze(image->pixels, image->height, image->width, (int)x, (int)y);

					// Save
					if (*total_descriptors < 255) {
						id_descriptors[*total_descriptors] = descriptor;
						x_descriptors[*total_descriptors] = x;
						y_descriptors[*total_descriptors] = y;
						(*total_descriptors)++;
						*is_descriptors_available = true;
					}
				}

				/* Show table */
				if (*is_descriptors_available) {
					// Create table header
					std::vector<std::vector<std::string>> table;
					table.push_back({ "Landmark number", "x", "y", "Descriptor"});
					for (uint8_t i = 0; i < *total_descriptors; i++) {
						table.push_back({ std::to_string(i), std::to_string(x_descriptors[i]),  std::to_string(y_descriptors[i]), std::to_string(id_descriptors[i])});
					

					}
					Tools_Gui_CreateItems_createTable("descriptor_table", table);
				}

				// Free
				imfree(image);
			}
		}

		// End window 
		ImGui::End();
	}
}