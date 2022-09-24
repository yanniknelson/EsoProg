#pragma once

//ImGui imports
#include <imgui.h>
#include <imgui_stdlib.h>

//Other imports
#include <stdio.h>
#include <iostream>
#include <fstream>


//Custom classes
#include "File_Dialog_Box/FileDialogBox.h"
#include "Resources/IconsFontAwesome6.h"
#include "Interpreter/Runtime.h"

class Program {

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
	
	//file management
	bool file_is_new = true;
	fs::path currentFilePath = "";

	//Interpreting
	Tokeniser tk;
	bool is_token_error = false;
	int token_error_line = 0;
	Runtime runtime;

	ImGuiInputTextFlags code_editor_flags = ImGuiInputTextFlags_CallbackCharFilter | ImGuiInputTextFlags_EnterReturnsTrue;
	std::string code{ "" };

public:

	Program() {
		//setup the current directory as the initial path in the file dialog box
		FileDialogBox::Init_Path(fs::current_path());

		//Merge font awesome into the default font
		ImGuiIO io = ImGui::GetIO();
		io.Fonts->AddFontDefault();
		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
		ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
		io.Fonts->AddFontFromFileTTF("fa-solid-900.ttf", 16.0f, &icons_config, icons_ranges);
	}

	void Render();
};