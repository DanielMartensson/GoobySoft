#include "ODORPDialog.h"
#include "../../../../Tools/Tools.h"

#define MIN_ODORP_PARAMETER_VALUE 0.0000001f

void Windows_Dialogs_UtilDialogs_ODORPDialog_showODORPDialog(bool* odorpDialog) {
	// Display
	ImGui::SetNextWindowSize(ImVec2(500, 350));
	if (ImGui::Begin("ODORP", odorpDialog, ImGuiWindowFlags_NoResize)) {

		// Get the parameter holder 
		ParameterHolder* parameterHolder = Tools_Hardware_ParameterStore_getParameterHolder();

		/* Extract */
		DATA_SETTINGS_ODORP* settings_ordorp = &parameterHolder->data_settings.data_settings_odorp;

		// Source to the data 
		static bool isOpen = false;
		ImGui::InputText(" ", settings_ordorp->folder_path, sizeof(settings_ordorp->folder_path), ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("Select folder")) {
			isOpen = true;
		}
		bool isPressedOK = false;
		static std::string folderPath;
		Tools_Gui_CreateDialogs_showFolderDialog(&isOpen, &isPressedOK, folderPath);
		if (isPressedOK) {
			std::strcpy(settings_ordorp->folder_path, folderPath.c_str());
			isOpen = false;
		}
		ImGui::Separator();

		// Sigma 1
		ImGui::InputFloat("Sigma 1", &settings_ordorp->sigma1);
		if (settings_ordorp->sigma1 < MIN_ODORP_PARAMETER_VALUE) {
			settings_ordorp->sigma1 = MIN_ODORP_PARAMETER_VALUE;
		}
		if (settings_ordorp->sigma1 > settings_ordorp->sigma2) {
			settings_ordorp->sigma1 = settings_ordorp->sigma2;
		}

		// Sigma 2
		ImGui::InputFloat("Sigma 2", &settings_ordorp->sigma2);
		if (settings_ordorp->sigma2 < MIN_ODORP_PARAMETER_VALUE) {
			settings_ordorp->sigma2 = MIN_ODORP_PARAMETER_VALUE;
		}

		// Threshold sobel
		int threshold_sobel_int = settings_ordorp->threshold_sobel;
		ImGui::InputInt("Sobel threshold", &threshold_sobel_int);
		if (threshold_sobel_int < 0) {
			threshold_sobel_int = 0;
		}
		else if (threshold_sobel_int > 255) {
			threshold_sobel_int = 255;
		}
		settings_ordorp->threshold_sobel = threshold_sobel_int;

		// Threshold FAST
		int threshold_fast_int = settings_ordorp->threshold_fast;
		ImGui::InputInt("FAST threshold", &threshold_fast_int);
		if (threshold_fast_int < 0) {
			threshold_fast_int = 0;
		}
		else if (threshold_fast_int > 255) {
			threshold_fast_int = 255;
		}
		settings_ordorp->threshold_fast = threshold_fast_int;

		// FAST method
		int fast_method_index;
		FAST_METHOD fast_method[4] = { FAST_METHOD_9, FAST_METHOD_10, FAST_METHOD_11, FAST_METHOD_12 };
		switch (settings_ordorp->fast_method) {
		case FAST_METHOD_9:
			fast_method_index = 0;
			break;
		case FAST_METHOD_10:
			fast_method_index = 1;
			break;
		case FAST_METHOD_11:
			fast_method_index = 2;
			break;
		case FAST_METHOD_12:
			fast_method_index = 3;
			break;
		default:
			fast_method_index = 0;
			break;
		}
		ImGui::Combo("FAST method", &fast_method_index, "FAST method 9\0FAST method 10\0FAST method 11\0FAST method 12");
		settings_ordorp->fast_method = fast_method[fast_method_index];
	
		ImGui::Separator();

		// Neural network settings
		ImGui::InputFloat("SVM C value", &settings_ordorp->C);
		if (settings_ordorp->C < MIN_ODORP_PARAMETER_VALUE) {
			settings_ordorp->C = MIN_ODORP_PARAMETER_VALUE;
		}
		ImGui::InputFloat("SVM lambda value", &settings_ordorp->lambda);
		if (settings_ordorp->lambda < MIN_ODORP_PARAMETER_VALUE) {
			settings_ordorp->lambda = MIN_ODORP_PARAMETER_VALUE;
		}

		// K-value
		ImGui::Separator();
		int k_value = settings_ordorp->k_value;
		ImGui::InputInt("K-nearest neighbor", &k_value);
		if (k_value < 2) {
			k_value = 2;
		}
		settings_ordorp->k_value = k_value;
		
		// Save model
		ImGui::Separator();
		ImGui::Checkbox("Save model", &settings_ordorp->save_model);

		// Build model
		ImGui::SameLine();
		if (ImGui::Button("Build model")) {
			ImGui::OpenPopup("buildFisherModel");
		}
		if (Tools_Gui_CreateDialogs_showPopUpConfirmDialogOKCancle("buildFisherModel", "Do you want to build model?", nullptr)) {
			/* Get data */
			DATA_COLLECT* data_collect = odorp(&parameterHolder->data_settings);

			/* Insert new data */
			for (int i = 0; i < parameterHolder->models.odorp_models.total_models; i++) {
				/* Free data */
				std::free(parameterHolder->models.odorp_models.model_b[i]);
				std::free(parameterHolder->models.odorp_models.model_w[i]);

				/* And then create data */
				parameterHolder->models.odorp_models.model_b[i] = (float*)malloc(data_collect->odorp_models.model_row[i] * sizeof(float));
				parameterHolder->models.odorp_models.model_w[i] = (float*)malloc(data_collect->odorp_models.model_row[i] * data_collect->odorp_models.model_column[i] * sizeof(float));

				/* Copy over */
				std::memcpy(parameterHolder->models.odorp_models.model_b[i], data_collect->odorp_models.model_b[i], data_collect->odorp_models.model_row[i] * sizeof(float));
				std::memcpy(parameterHolder->models.odorp_models.model_w[i], data_collect->odorp_models.model_w[i], data_collect->odorp_models.model_row[i] * data_collect->odorp_models.model_column[i] * sizeof(float));

				/* Update parameters */
				parameterHolder->models.odorp_models.activation_function[i] = data_collect->odorp_models.activation_function[i];
				parameterHolder->models.odorp_models.model_column[i] = data_collect->odorp_models.model_column[i];
				parameterHolder->models.odorp_models.model_row[i] = data_collect->odorp_models.model_row[i];
			}

			/* Total models */
			parameterHolder->models.odorp_models.total_models = data_collect->odorp_models.total_models;
		
			/* Clear */
			imcollect_free(data_collect);
		}

		// End window 
		ImGui::End();
	}
}
