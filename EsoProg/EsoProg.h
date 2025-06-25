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
#include "PietInterpreter/PietRuntime.h"
#include "SmartEnum.h"


class EsoProg {

#define EFILETYPES(x)\
    x(Text)\
    x(Image)

	CreateSmartEnum(EFileType, EFILETYPES);

#undef EFILETYPES

#define ELANGUAGES(x)\
    x(Piet)

	CreateSmartEnum(ELanguages, ELANGUAGES);

#undef ELANGUAGES

	void CheckShortCuts();

	void HandleNew();
	void HandleOpen();
	void HandelSaveAs();
	void HandleSave();

	void CreateMenuBar();
	
	//shortcut flags
	bool m_bShortcutUsed = false;
	ImGuiKey m_lastShortcutUsed = ImGuiKey_None;

	//file dialog box flags
	bool m_bEnableFileDialog = false;
	FileDialogBox::FileDialogType m_dialogType = FileDialogBox::FileDialogType::Open;
	ImGuiWindowFlags_ m_fileDialogOnTop = ImGuiWindowFlags_None;
	std::fstream m_fileStream;

	EFileType::Enum m_currentFileType = EFileType::COUNT;

	void PreFileLoad(const std::filesystem::path path);
	EFileType::Enum LoadFile(const std::filesystem::path path);
	void PostFileLoad(const EFileType::Enum fileType, const std::filesystem::path path);
	
	ELanguages::Enum m_currentLanguage = ELanguages::COUNT;

	void SetCurrentLanugage(ELanguages::Enum languag);

	//file management
	bool m_bFileIsNew = true;
	fs::path m_currentFilePath = "";

	//Image
	unsigned char* m_imageData{ nullptr };
	int m_imageWidth = 0;
	int m_imageHeight = 0;
	int m_NumComponents = 0;
	bool m_bImageLoaded = false;
	float m_aspectRatio = 1.f;

	GLuint m_texture;

	//Interpreting
	PietTextTokeniser m_textValidationTokeniser;
	bool m_bIsTokenError = false;
	bool m_bVerificationAttempted = false;
	int m_tokenErrorLine = 0;
	std::ostringstream m_outputStream{ "" };
	std::ostringstream m_executionHistoryStream{ "" };
	Runtime m_runtime{ m_outputStream, m_executionHistoryStream };

	ImGuiInputTextFlags m_codeEditorFlags = ImGuiInputTextFlags_CallbackCharFilter | ImGuiInputTextFlags_EnterReturnsTrue;
	std::string m_code{ "" };
	std::string m_programInput{ "" };

public:
	static char* i_ProgramName;
	static GLFWwindow* i_pWindow;

	EsoProg(GLFWwindow* pWindow)
	{
		i_pWindow = pWindow;
		//setup the current directory as the initial path in the file dialog box
		FileDialogBox::Init_Path(fs::current_path());
		//SetCurrentLanugage(ELanguages::Piet);

		glGenTextures(1, &m_texture);

		//Merge font awesome into the default font
		ImGuiIO io = ImGui::GetIO();
		io.Fonts->AddFontDefault();
		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
		ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
		io.Fonts->AddFontFromFileTTF("fa-solid-900.ttf", 16.0f, &icons_config, icons_ranges);
	}

	void Update();
};