#include "CameraDialog.h"
#include "../../../../../Tools/Tools.h"
#include <opencv4/opencv2/opencv.hpp>

void Windows_Dialogs_UtilDialogs_ImageDialogs_CameraDialog_showCameraDialog(bool* cameraDialog) {
	//  Display
	if (ImGui::Begin("Camera", cameraDialog)) {
		// Open the camera
		static cv::VideoCapture cap;
		if (cap.isOpened()) {
			if (ImGui::Button("Disconnect")) {
				cap.release();
				ImGui::OpenPopup("Disconnected");
			}
		}
		else {
			static int camIndex;
			ImGui::PushItemWidth(60);
			ImGui::Combo("Camera device", &camIndex, "Cam0\0Cam1\0Cam2\0Cam3\0");
			ImGui::SameLine();
			if (ImGui::Button("Connect")) {
				if (cap.open(camIndex, cv::CAP_DSHOW)) {
					ImGui::OpenPopup("Connected");
				}
				else {
					ImGui::OpenPopup("Failed to connected");
				}
			}
		}

		// Pop up boxes
		Tools_Gui_CreateDialogs_showPopUpInformationDialogOK("Connected", "You are connected to the camera");
		Tools_Gui_CreateDialogs_showPopUpInformationDialogOK("Disconnected", "You are disconnected from the camera");
		Tools_Gui_CreateDialogs_showPopUpInformationDialogOK("Failed to connect", "Could not connect to the camera");

		// Source to the data 
		ImGui::SameLine();
		ImGui::PushItemWidth(400);
		static bool isOpen = false;
		static std::string folderPath;
		ImGui::InputText(" ", (char*)folderPath.c_str(), folderPath.length(), ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		if (ImGui::Button("Select folder")) {
			isOpen = true;
		}
		bool isPressedOK = false;
		Tools_Gui_CreateDialogs_showFolderDialog(&isOpen, &isPressedOK, folderPath);

		// Campute
		if (cap.isOpened()) {
			// Capture frame
			cv::Mat frame;
			cap >> frame;

			// Turn image to grey scale 
			cv::Mat image;
			cv::cvtColor(frame, image, cv::COLOR_RGB2GRAY);

			// Create display image
			static GLuint texID;
			glDeleteTextures(1, &texID);
			glGenTextures(1, &texID);
			glBindTexture(GL_TEXTURE_2D, texID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Save capute
			ImGui::SameLine();
			if (folderPath.length() > 3) {
				if (ImGui::Button("Capture")) {
					std::string fileName = Tools_Software_Algorithms_getISO8601Time() + ".pgm";
					std::replace(fileName.begin(), fileName.end(), ':', '_');
					if (!cv::imwrite(folderPath + "\\" + fileName, image)) {
						ImGui::OpenPopup("Failed to save");
					}
				}
			}
			Tools_Gui_CreateDialogs_showPopUpInformationDialogOK("Failed to save", "Could not capure and save image");
			ImGui::Separator();

			// Create texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.data);

			// Dipslay image
			ImGui::NewLine();
			ImGui::Image((ImTextureID)texID, ImVec2(image.cols, image.rows));
		}

		// End
		ImGui::End();
	}
}