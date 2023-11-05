#include "FisherFacesDialog.h"
#include "../../../../Tools/Tools.h"

#define MIN_FISHERFACES_PARAMETER_VALUE 0.0000001f

void Windows_Dialogs_UtilDialogs_FisherFacesDialog_showFisherFacesDialog(bool* fisherFacesDialog) {
	// Display
	ImGui::SetNextWindowSize(ImVec2(500, 350));
	if (ImGui::Begin("Fisherfaces", fisherFacesDialog, ImGuiWindowFlags_NoResize)) {

		// Get the parameter holder 
		ParameterHolder* parameterHolder = Tools_Hardware_ParameterStore_getParameterHolder();

		/* Extract */
		DATA_SETTINGS_FISHERFACES* settings_fisherfaces = &parameterHolder->data_settings.data_settings_fisherfaces;

		// Source to the data 
		static bool isOpen = false;
		ImGui::InputText(" ", settings_fisherfaces->folder_path, sizeof(settings_fisherfaces->folder_path), ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("Select folder")) {
			isOpen = true;
		}
		bool isPressedOK = false;
		static std::string folderPath;
		Tools_Gui_CreateDialogs_showFolderDialog(&isOpen, &isPressedOK, folderPath);
		if (isPressedOK) {
			std::strcpy(settings_fisherfaces->folder_path, folderPath.c_str());
			isOpen = false;
		}
		ImGui::Separator();

		// Pooling size 
		int pooling_size = settings_fisherfaces->pooling_size;
		ImGui::InputInt("Pooling size", &pooling_size);
		if (pooling_size < 1) {
			pooling_size = 1;
		}
		settings_fisherfaces->pooling_size = pooling_size;

		// Pooling method
		static int pooling_method_index;
		POOLING_METHOD pooling_method[4] = { POOLING_METHOD_NO_POOLING, POOLING_METHOD_MAX, POOLING_METHOD_AVERAGE, POOLING_METHOD_SHAPE };
		switch (settings_fisherfaces->pooling_method) {
		case POOLING_METHOD_NO_POOLING:
			pooling_size = 1;
			pooling_method_index = 0;
			break;
		case POOLING_METHOD_MAX:
			pooling_method_index = 1;
			break;
		case POOLING_METHOD_AVERAGE:
			pooling_method_index = 2;
			break;
		case POOLING_METHOD_SHAPE:
			pooling_method_index = 3;
			break;
		default:
			pooling_size = 1;
			pooling_method_index = 0;
			break;
		}
		ImGui::Combo("Pooling method", &pooling_method_index, "No pooling\0Max pooling\0Average pooling\0Shape pooling");
		settings_fisherfaces->pooling_method = pooling_method[pooling_method_index];

		ImGui::Separator();
		// Remove outliers settings
		ImGui::Checkbox("Remove outliers", &settings_fisherfaces->remove_outliers);
		if (settings_fisherfaces->remove_outliers) {
			// Epsilon
			ImGui::InputFloat("Epsilon", &settings_fisherfaces->epsilon);
			if (settings_fisherfaces->epsilon < MIN_FISHERFACES_PARAMETER_VALUE) {
				settings_fisherfaces->epsilon = MIN_FISHERFACES_PARAMETER_VALUE;
			}

			// Min PTS
			int min_pts = settings_fisherfaces->min_pts;
			ImGui::InputInt("Min PTS", &min_pts);
			if (min_pts < 1) {
				min_pts = 1;
			}
			settings_fisherfaces->min_pts = min_pts;
		}
		ImGui::Separator();

		// Components PCA
		int components_pca = settings_fisherfaces->components_pca;
		ImGui::InputInt("Components Kernel PCA", &components_pca);
		if (components_pca < 1) {
			components_pca = 1;
		}
		settings_fisherfaces->components_pca = components_pca;

		// Kernel method
		static int kernel_method_index;
		KERNEL_METHOD kernel_method[6] = { KERNEL_METHOD_LINEAR, KERNEL_METHOD_RBF, KERNEL_METHOD_POLY, KERNEL_METHOD_SIGMOID, KERNEL_METHOD_GAUSSIAN, KERNEL_METHOD_EXPONENTIAL };
		switch (settings_fisherfaces->kernel_method) {
		case KERNEL_METHOD_LINEAR:
			kernel_method_index = 0;
			break;
		case KERNEL_METHOD_RBF:
			kernel_method_index = 1;
			break;
		case KERNEL_METHOD_POLY:
			kernel_method_index = 2;
			break;
		case KERNEL_METHOD_SIGMOID:
			kernel_method_index = 3;
			break;
		case KERNEL_METHOD_GAUSSIAN:
			kernel_method_index = 4;
			break;
		case KERNEL_METHOD_EXPONENTIAL:
			kernel_method_index = 5;
			break;
		default:
			kernel_method_index = 0;
			break;
		}
		ImGui::Combo("Kernel method", &kernel_method_index, "Linear\0Radial basis function\0Polynomial\0Sigmoid\0Gaussian\0Exponential");
		settings_fisherfaces->kernel_method = kernel_method[kernel_method_index];
		switch (settings_fisherfaces->kernel_method) {
		case KERNEL_METHOD_LINEAR:
			break;
		case KERNEL_METHOD_RBF:
			ImGui::InputFloat("Gamma", &settings_fisherfaces->kernel_parameters[0], 0, 0, "%.7f");
			break;
		case KERNEL_METHOD_POLY:
			ImGui::InputFloat("Degree", &settings_fisherfaces->kernel_parameters[0], 0, 0, "%.7f");
			break;
		case KERNEL_METHOD_SIGMOID:
			ImGui::InputFloat("Alpha", &settings_fisherfaces->kernel_parameters[0], 0, 0, "%.7f");
			ImGui::InputFloat("Beta", &settings_fisherfaces->kernel_parameters[1], 0, 0, "%.7f");
			break;
		case KERNEL_METHOD_GAUSSIAN:
			ImGui::InputFloat("Sigma", &settings_fisherfaces->kernel_parameters[0], 0, 0, "%.7f");
			break;
		case KERNEL_METHOD_EXPONENTIAL:
			ImGui::InputFloat("Sigma", &settings_fisherfaces->kernel_parameters[0], 0, 0, "%.7f");
			break;
		}
		for (int i = 0; i < 2; i++) {
			if (settings_fisherfaces->kernel_parameters[i] < MIN_FISHERFACES_PARAMETER_VALUE) {
				settings_fisherfaces->kernel_parameters[i] = MIN_FISHERFACES_PARAMETER_VALUE;
			}
		}
		ImGui::Separator();

		// Neural network settings
		ImGui::InputFloat("SVM C value", &settings_fisherfaces->C);
		if (settings_fisherfaces->C < MIN_FISHERFACES_PARAMETER_VALUE) {
			settings_fisherfaces->C = MIN_FISHERFACES_PARAMETER_VALUE;
		}
		ImGui::InputFloat("SVM lambda value", &settings_fisherfaces->lambda);
		if (settings_fisherfaces->lambda < MIN_FISHERFACES_PARAMETER_VALUE) {
			settings_fisherfaces->lambda = MIN_FISHERFACES_PARAMETER_VALUE;
		}

		// Save model
		ImGui::Separator();
		ImGui::Checkbox("Save model", &settings_fisherfaces->save_model);

		// Build model
		ImGui::SameLine();
		if (ImGui::Button("Build model")) {
			ImGui::OpenPopup("buildFisherModel");
		}
		if (Tools_Gui_CreateDialogs_showPopUpConfirmDialogOKCancle("buildFisherModel", "Do you want to build model?", nullptr)) {
			/* Get data */
			DATA_COLLECT* data_collect = fisherfaces(&parameterHolder->data_settings);
			
			/* Insert new data */
			for (int i = 0; i < parameterHolder->models.fisherfaces_models.total_models; i++) {
				/* Free data */
				std::free(parameterHolder->models.fisherfaces_models.model_b[i]);
				std::free(parameterHolder->models.fisherfaces_models.model_w[i]);

				/* And then create data */
				parameterHolder->models.fisherfaces_models.model_b[i] = (float*)malloc(data_collect->fisherfaces_models.model_row[i] * sizeof(float));
				parameterHolder->models.fisherfaces_models.model_w[i] = (float*)malloc(data_collect->fisherfaces_models.model_row[i] * data_collect->fisherfaces_models.model_column[i] * sizeof(float));

				/* Copy over */
				std::memcpy(parameterHolder->models.fisherfaces_models.model_b[i], data_collect->fisherfaces_models.model_b[i], data_collect->fisherfaces_models.model_row[i] * sizeof(float));
				std::memcpy(parameterHolder->models.fisherfaces_models.model_w[i], data_collect->fisherfaces_models.model_w[i], data_collect->fisherfaces_models.model_row[i] * data_collect->fisherfaces_models.model_column[i] * sizeof(float));

				/* Update parameters */
				parameterHolder->models.fisherfaces_models.activation_function[i] = data_collect->fisherfaces_models.activation_function[i];
				parameterHolder->models.fisherfaces_models.model_column[i] = data_collect->fisherfaces_models.model_column[i];
				parameterHolder->models.fisherfaces_models.model_row[i] = data_collect->fisherfaces_models.model_row[i];
			}

			/* Total models */
			parameterHolder->models.fisherfaces_models.total_models = data_collect->fisherfaces_models.total_models;
			
			/* Clear */
			imcollect_free(data_collect);
		}

		// End window 
		ImGui::End();
	}
}
