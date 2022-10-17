#pragma once

//ImGui imports
#include <imgui.h>
#include <imgui_stdlib.h>

//OpenGL imports
#include "glad/glad.h"
#include "GLFW/glfw3.h"

//Other imports
#include <stdio.h>
#include <iostream>
#include <fstream>


//Custom classes
#include "File_Dialog_Box/FileDialogBox.h"
#include "Resources/IconsFontAwesome6.h"
#include "PietInterpreter/Runtime.h"


class Program {

	enum FileType {
		Text,
		Image
	};

	static int Check_For_Enter(ImGuiInputTextCallbackData* data);
	void Check_ShortCuts();

	void handle_New();
	void handle_Open();
	void handle_Save_As();
	void handle_Save();

	void CreateMenuBar();
	
	//shortcut flags
	bool shortcut_used = false;
	ImGuiKey_ last_shortcut_used = ImGuiKey_None;

	//file dialog box flags
	bool enable_file_dialog = false;
	FileDialogBox::FileDialogType dialogType = FileDialogBox::FileDialogType::Open;
	ImGuiWindowFlags_ file_dialog_on_top = ImGuiWindowFlags_None;
	std::fstream fs;

	FileType OpenFile(std::filesystem::path path);
	
	//file management
	bool file_is_new = true;
	fs::path currentFilePath = "";

	//Image
	unsigned char* image_data;
	int image_width = 0;
	int image_height = 0;
	int num_components = 0;
	bool image_loaded = false;
	float aspect_ratio = 1.f;

	GLuint texture;

	//Interpreting
	PietTextTokeniser tk;
	bool is_token_error = false;
	bool is_compiled = false;
	int token_error_line = 0;
	std::string output = "";
	Runtime runtime{ output };

	ImGuiInputTextFlags code_editor_flags = ImGuiInputTextFlags_CallbackCharFilter | ImGuiInputTextFlags_EnterReturnsTrue;
	std::string code{ "" };

public:

	Program() {
		//setup the current directory as the initial path in the file dialog box
		FileDialogBox::Init_Path(fs::current_path());
		FileDialogBox::Set_Allowed_Type({".txt", ".jpg", ".png", ".gif", ".ppm"});

		glGenTextures(1, &texture);

		//Merge font awesome into the default font
		ImGuiIO io = ImGui::GetIO();
		io.Fonts->AddFontDefault();
		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
		ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
		io.Fonts->AddFontFromFileTTF("fa-solid-900.ttf", 16.0f, &icons_config, icons_ranges);
	}

	void Render();
};