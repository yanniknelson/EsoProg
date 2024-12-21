#include "EsoProg.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Resources/stb_image.h"

char* EsoProg::i_ProgramName = "EsoProg";

int EsoProg::TextInputCallback(ImGuiInputTextCallbackData* data)
{
	*((bool*)data->UserData) = false;

	//don't change the input key
	return 0;
}

int EsoProg::ValueInputChanged(ImGuiInputTextCallbackData* data)
{
	*((int*)data->UserData) = std::stoi(data->Buf);

	return 0;
}

void EsoProg::HandleNew()
{
	m_bFileIsNew = true;
	m_code = "";
}

void EsoProg::HandleOpen()
{
	m_bEnableFileDialog = true;
	m_dialogType = FileDialogBox::FileDialogType::Open;
}

void EsoProg::HandleSave()
{
	if (m_bFileIsNew)
	{
		HandelSaveAs();
	}
	else
	{
		std::ofstream fileStream;
		fileStream.open(m_currentFilePath.string());
		fileStream << m_code;
		fileStream.close();
	}
}

void EsoProg::HandelSaveAs()
{
	m_bEnableFileDialog = true;
	m_dialogType = FileDialogBox::FileDialogType::Save_As;
}

void EsoProg::CheckShortCuts()
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

void EsoProg::CreateMenuBar()
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

EsoProg::FileType EsoProg::OpenFile(std::filesystem::path path)
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
		m_runtime.SetImage(m_imageData, m_imageWidth, m_imageHeight);

		return FileType::Image;
	}
}

void EsoProg::Render()
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

		if (ImGui::InputTextMultiline("##code", &m_code, ImVec2(-FLT_MIN, ImGui::GetContentRegionAvail().y - (ImGui::GetTextLineHeight() * 1.5)), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackEdit, TextInputCallback, &m_bVerificationAttempted))
		{
			std::cout << "code changed" << std::endl;
			m_bVerificationAttempted = false;
		}

		if (ImGui::Button("Verify"))
		{
			m_bVerificationAttempted = true;
			m_bIsTokenError = false;
			std::stringstream input(m_code.c_str());
			m_textValidationTokeniser.SetTextStream(input);
			PietToken token = Runtime::m_tDefaultToken;

			while (token.m_type != PietToken::TokenType::End)
			{
				//std::cout << m_textValidationTokeniser.get_line_number() << " " << token << std::endl;
				token = m_textValidationTokeniser.Pop();
				if (token.m_type == PietToken::TokenType::Unrecognised_Token)
				{
					m_bIsTokenError = true;
					m_tokenErrorLine = m_textValidationTokeniser.GetLineNumber();
					break;
				}
			}

			if (!m_bIsTokenError)
			{
				m_runtime.SetTextStream(m_code);
			}
		}

		if (m_bVerificationAttempted)
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
					m_isStepping = false;
					m_runtime.RunFromStart(Runtime::SourceType::Text); // add a run speed
				}

				ImGui::SameLine();
				if (ImGui::Button("Pause"))
				{

				}

				ImGui::SameLine();
				if (ImGui::Button("Step"))
				{
					m_isStepping = true;
					m_runtime.StepExecution(Runtime::SourceType::Text);
				}

				if (m_runtime.IsFinished())
				{
					ImGui::SameLine();
					if (ImGui::Button("Reset"))
					{
						m_runtime.Reset();
					}
				}
			}
		}
		ImGui::End();
	}

	// Input Value Box --------------------------------------------------------------------------------------------
	if (m_runtime.IsWaitingForValInput())
	{
		if (ImGui::Begin("Input Val"))
		{
			ImGui::InputText("##valInput", &m_programInput, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CallbackEdit);
			if (ImGui::Button("Submit"))
			{
				m_runtime.InputVal(std::stoi(m_programInput));
				m_programInput = "";
				if (!m_isStepping)
				{
					m_runtime.Run();
				}
			}
			ImGui::End();
		}
	}

	// Input Char Box --------------------------------------------------------------------------------------------
	if (m_runtime.IsWaitingForCharInput())
	{
		if (ImGui::Begin("Input Char"))
		{
			ImGui::InputText("##charInput", &m_programInput, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CallbackEdit);
			if (ImGui::Button("Submit (will only submit first character)"))
			{
				m_runtime.InputChar((char)m_programInput[0]);
				m_programInput = "";
				if (!m_isStepping)
				{
					m_runtime.Run();
				}
			}
			ImGui::End();
		}
	}

	// IMAGE PROGRAM DISPLAY --------------------------------------------------------------------------------------------
	if (ImGui::Begin("Piet Image"))
	{
		if (m_bImageLoaded)
		{
			const ImVec2 area = ImGui::GetContentRegionAvail();
			ImVec2 desired = ImVec2(area.x, (int)area.x * m_aspectRatio);
			if (desired.y > area.y)
			{
				desired = ImVec2((int)area.y * (1 / m_aspectRatio), area.y);
			}
			ImGui::Image((void*)(intptr_t)m_texture, desired);
		}

		if (ImGui::Button("Run"))
		{
			m_isStepping = false;
			m_runtime.RunFromStart(Runtime::SourceType::Image);
		}

		ImGui::SameLine();
		if (ImGui::Button("Step"))
		{
			m_isStepping = true;
			m_runtime.StepExecution(Runtime::SourceType::Image);
		}

		ImGui::SameLine();
		{
			int newCodelSize = m_codelSize;
			ImGui::InputText("##codelSize", &m_codelSizeStr, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CallbackEdit, ValueInputChanged, &newCodelSize);
			if (newCodelSize != m_codelSize)
			{
				m_codelSize = newCodelSize;
				m_runtime.SetCodelSize(m_codelSize);
			}
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
			const std::deque<int>& rStack = m_runtime.GetStack();
			for (int i = rStack.size() - 1; i >= 0; i--)
			{
				std::string lbl = std::to_string(rStack[i]) + "##" + std::to_string(i);
				ImGui::Selectable(lbl.c_str(), false, ImGuiSelectableFlags_Disabled);
			}
			ImGui::EndListBox();
		}
		ImGui::End();
	}

	// INSTRUCTION HISTORY -----------------------------------------------------------------------------------------------
	bool displayInstructionHistory = true;
	{
		if (ImGui::Begin("Execution History", &displayInstructionHistory, m_fileDialogOnTop))
		{
			const std::string tmpExecHist = m_executionHistoryStream.str();
			ImGui::TextWrapped(tmpExecHist.c_str());
			ImGui::End();
		}
	}

	// PROGRAM OUTPUT ----------------------------------------------------------------------------------------------------
	bool outputDisplayOpen = true;
	if (ImGui::Begin("Program Output:", &outputDisplayOpen, m_fileDialogOnTop))
	{
		const std::string tmpOuput = m_outputStream.str();
		ImGui::TextWrapped(tmpOuput.c_str());
		ImGui::End();
	}
}