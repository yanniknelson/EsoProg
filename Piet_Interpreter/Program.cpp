#include "Program.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Resources/stb_image.h"

int Program::CheckForEnter(ImGuiInputTextCallbackData* data)
{
	//if the key pressed was enter (catching windows and linux new lines) set the userdata flag to true
	if (data->EventChar == 10 || data->EventChar == 13)
	{
		*((bool*)data->UserData) = true;
	}

	//don't change the input key
	return 0;
}

void Program::HandleNew()
{
	//std::cout << "New\n";
	m_bFileIsNew = true;
	m_code = "";
}

void Program::HandleOpen()
{
	//std::cout << "Open\n";
	m_bEnableFileDialog = true;
	m_dialogType = FileDialogBox::FileDialogType::Open;
}

void Program::HandleSave()
{
	if (m_bFileIsNew)
	{
		HandelSaveAs();
	}
	else
	{
		//std::cout << "Save\n";
		std::ofstream fileStream;
		fileStream.open(m_currentFilePath.string());
		fileStream << m_code;
		fileStream.close();
	}
}

void Program::HandelSaveAs()
{
	//std::cout << "Save As\n";
	m_bEnableFileDialog = true;
	m_dialogType = FileDialogBox::FileDialogType::Save_As;
}

void Program::CheckShortCuts()
{
	const ImGuiIO& io = ImGui::GetIO();
	//if a shortcut has just been used and either ctrl or the alpha key is up then enable the triggering of a new shortcut
	if (m_bShortcutUsed && (!io.KeyCtrl || !io.KeysDown[m_lastShortcutUsed]))
	{
		m_bShortcutUsed = false;
	}

	//if we have dissabled the triggering of a new shortcut return false
	if (m_bShortcutUsed)
	{
		return;
	}

	//check the combo for a shortcut, disable new shortcuts, store the used alpha key and handle the shortcut
	if (io.KeyCtrl)
	{
		if (io.KeysDown[ImGuiKey_N])
		{
			m_bShortcutUsed = true;
			m_lastShortcutUsed = ImGuiKey_N;
			HandleNew();
		}
		else if (io.KeysDown[ImGuiKey_O])
		{
			m_bShortcutUsed = true;
			m_lastShortcutUsed = ImGuiKey_O;
			HandleOpen();
		}
		else if (io.KeyShift && io.KeysDown[ImGuiKey_S])
		{
			m_bShortcutUsed = true;
			m_lastShortcutUsed = ImGuiKey_S;
			HandelSaveAs();
		}
		else if (io.KeysDown[ImGuiKey_S])
		{
			m_bShortcutUsed = true;
			m_lastShortcutUsed = ImGuiKey_S;
			HandleSave();
		}
	}

}

void Program::CreateMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		//file menu
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem(ICON_FA_FILE_CIRCLE_PLUS " New", "Ctrl+N"))
			{
				HandleNew();
			}

			if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN " Open", "Ctrl+O"))
			{
				HandleOpen();
			}

			if (ImGui::MenuItem(ICON_FA_FLOPPY_DISK " Save", "Ctrl+S"))
			{
				HandleSave();
			}

			if (ImGui::MenuItem(ICON_FA_FLOPPY_DISK " Save As", "Ctrl+Shift+S"))
			{
				HandelSaveAs();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

Program::FileType Program::OpenFile(std::filesystem::path path)
{
	if (path.extension() == ".txt")
	{
		m_fileStream.open(path.string(), m_fileStream.in);
		m_code = "";
		std::getline(m_fileStream, m_code, char(0));
		m_bFileIsNew = false;
		m_currentFilePath = path;
		m_fileStream.close();
		return FileType::Text;
	}
	else
	{
		if (m_bImageLoaded)
		{
			stbi_image_free(m_imageData);
		}

		m_imageData = stbi_load(path.string().c_str(), &m_imageWidth, &m_imageHeight, &m_NumComponents, 4);
		if (m_imageData)
		{
			m_aspectRatio = (float)m_imageHeight / (float)m_imageWidth;
			glBindTexture(GL_TEXTURE_2D, m_texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
			glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_imageWidth, m_imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_imageData);
			glGenerateMipmap(GL_TEXTURE_2D);
			m_bImageLoaded = true;
		}

		return FileType::Image;
	}
}

void Program::Render()
{
	m_fileDialogOnTop = ImGuiWindowFlags_None;
	//check if any shortcut is active and handle it
	CheckShortCuts();

	CreateMenuBar();

	// FILE DIALOG BOX ------------------------------------------------------------------------------------------------------
	if (m_bEnableFileDialog)
	{
		//set the nobring to front on focus flag for all other elements to ensure
		//the file dialog is always on top
		m_fileDialogOnTop = ImGuiWindowFlags_NoBringToFrontOnFocus;
		FileDialogBox::FileDialogReturn ret = FileDialogBox::Create_File_Dialog(m_bEnableFileDialog, m_dialogType);
		if (ret.selected)
		{
			switch (m_dialogType)
			{
			case FileDialogBox::Open:
			{
				OpenFile(ret.path);
				break;
			}
			case FileDialogBox::Save_As:
			{
				m_fileStream.open(ret.path.string(), m_fileStream.out);
				m_fileStream << m_code;
				m_fileStream.close();
				m_bFileIsNew = false;
				m_currentFilePath = ret.path;
				break;
			}
			default:
				break;
			}
		}
	}

	// CODE EDITOR --------------------------------------------------------------------------------------------------
	bool code_editor_open = true;
	if (ImGui::Begin("Code Editor", &code_editor_open, m_fileDialogOnTop))
	{
		ImGui::Text("Warning: Pressing ESC will delete all non submitted changes");
		bool enterPressed = false;
		if (ImGui::InputTextMultiline("##code", &m_code, ImVec2(-FLT_MIN, ImGui::GetContentRegionAvail().y - (ImGui::GetTextLineHeight() * 1.5)), ImGuiInputTextFlags_CallbackCharFilter | ImGuiInputTextFlags_EnterReturnsTrue, CheckForEnter, &enterPressed))
		{
			//std::cout << code << std::endl;
		}

		if (enterPressed)
		{

		}

		if (ImGui::Button("Compile"))
		{
			m_bIsTokenError = false;
			std::istrstream input(m_code.c_str());
			m_textValidationTokeniser.SetStream(input);
			PietToken token = m_textValidationTokeniser.Pop();

			while (token.m_kind != PietToken::TokenType::End)
			{
				//std::cout << m_textValidationTokeniser.get_line_number() << " " << token << std::endl;
				token = m_textValidationTokeniser.Pop();
				if (token.m_kind == PietToken::TokenType::Unrecognised_Token)
				{
					m_bIsTokenError = true;
					m_tokenErrorLine = m_textValidationTokeniser.GetLineNumber();
					break;
				}
			}

			if (!m_bIsTokenError)
			{
				runtime.SetStream(m_code);
				m_bIsCompiled = true;
			}
		}

		if (m_bIsCompiled)
		{
			if (m_bIsTokenError)
			{
				ImGui::SameLine();
				ImGui::Text("There's an error on line %d, lower lines have not been checked", m_tokenErrorLine);
			}
			else
			{
				ImGui::SameLine();
				if (ImGui::Button("Run"))
				{
					m_output = "";
					runtime.Run(); // add a run speed
				}

				ImGui::SameLine();
				if (ImGui::Button("Pause"))
				{

				}

				ImGui::SameLine();
				if (ImGui::Button("Step"))
				{
					runtime.StepExecution();
				}

				if (runtime.IsFinished())
				{
					ImGui::SameLine();
					if (ImGui::Button("Reset"))
					{
						runtime.Reset();
					}
				}
			}
		}
		ImGui::End();
	}

	// IMAGE PROGRAM DISPLAY --------------------------------------------------------------------------------------------
	if (ImGui::Begin("OpenGL Texture Text"))
	{
		if (m_bImageLoaded)
		{
			ImVec2 area = ImGui::GetContentRegionAvail();
			ImVec2 desired = ImVec2(area.x, (int)area.x * m_aspectRatio);
			if (desired.y > area.y)
			{
				desired = ImVec2((int)area.y * (1 / m_aspectRatio), area.y);
			}
			ImGui::Image((void*)(intptr_t)m_texture, desired);
		}

		ImGui::End();
	}

	// STACK DISPLAY ----------------------------------------------------------------------------------------------------
	bool stackDisplayOpen = true;
	if (ImGui::Begin("Stack", &stackDisplayOpen, m_fileDialogOnTop))
	{
		ImGui::Text("The rStack is displayed with the deepest value at the top");
		if (ImGui::BeginListBox("##Stack", ImGui::GetContentRegionAvail()))
		{
			const std::deque<int>& rStack = runtime.GetStack();
			for (int i = rStack.size() - 1; i >= 0; i--)
			{
				std::string lbl = std::to_string(rStack[i]) + "##" + std::to_string(i);
				ImGui::Selectable(lbl.c_str(), false, ImGuiSelectableFlags_Disabled);
			}
			ImGui::EndListBox();
		}
		ImGui::End();
	}

	// PROGRAM OUTPUT ----------------------------------------------------------------------------------------------------
	bool output_display_open = true;
	if (ImGui::Begin("Program Output:", &output_display_open, m_fileDialogOnTop))
	{
		ImGui::TextWrapped(m_output.c_str());
		ImGui::End();
	}
}