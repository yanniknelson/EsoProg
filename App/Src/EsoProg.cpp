#pragma once

#include "EsoProg.h"

#include <ELanguages.h>                 // for ELanguages::Enum
#include <FileDialogBox.h>              // for CFileDialogBox
#include <ImGuiSetStyles.h>             // for SetImGuiDarkStyle, SetImGuiLightStyle
#include <ImGuiValueChangeCallbacks.h>  // for TextInputCallback, ValueInputChanged
#include <PietRuntime.h>                // for PietRuntime

#include <GLFW/glfw3.h>                 // for GLFWwindow
#include <IconsFontAwesome7.h>          // for ICON_MIN_FA, ICON_MAX_16_FA, ICON_FA_FILE_CIRCLE_PLUS, ICON_FA_FOLDER_OPEN, ICON_FA_FLOPPY_DISK
#include <gl/GL.h>                      // for glBindTexture, glTexParameteri, glPixelStorei, glTexImage2D
#include <imgui.h>                      // for all imgui functions
#include <imgui_stdlib.h>               // for InputText and others with std::string input
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>                  // for stbi_load, stbi_image_free

#include <cfloat>                       // for FLT_MIN
#include <filesystem>                   // for std::filesystem::path
#include <fstream>                      // for std::ofstream
#include <iostream>
#include <string>

const char* CEsoProg::s_ProgramName = "EsoProg";
GLFWwindow* CEsoProg::s_pWindow = nullptr;

//////////////////////////////////////////////////////////////
CEsoProg::CEsoProg(GLFWwindow* pWindow)
{
    s_pWindow = pWindow;
    //setup the current directory as the initial path in the file dialog box
    CFileDialogBox::Init_Path(fs::current_path());
    //SetCurrentLanugage(ELanguages::Piet);

    glGenTextures(1, &m_texture);

    m_pRuntime = &m_nullRuntime;

    //Merge font awesome into the default font
    ImGuiIO io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    io.Fonts->AddFontFromFileTTF("../Vendor/Font-Awesome/otfs/Font Awesome 7 Free-Solid-900.otf", 16.0f, &icons_config, icons_ranges);
}

//////////////////////////////////////////////////////////////
void CEsoProg::Render()
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
        CFileDialogBox::SFileDialogReturn ret = CFileDialogBox::Create_File_Dialog(m_bEnableFileDialog, m_dialogType);
        if (ret.m_bSelected)
        {
            switch (m_dialogType)
            {
            case CFileDialogBox::Open:
            {
                const EFileType::Enum fileType = LoadFile(ret.m_path);
                break;
            }
            case CFileDialogBox::Save_As:
            {
                m_fileStream.open(ret.m_path.string(), m_fileStream.out);
                m_fileStream << m_code;
                m_fileStream.close();
                m_bFileIsNew = false;
                m_currentFilePath = ret.m_path;
                break;
            }
            default:
                break;
            }
        }
    }

    // Input Value Box --------------------------------------------------------------------------------------------
    if (m_pRuntime->IsWaitingForValInput())
    {
        if (ImGui::Begin("Input Val"))
        {
            ImGui::InputText("##valInput", &m_programInput, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CallbackEdit);
            if (ImGui::Button("Submit"))
            {
                m_pRuntime->InputVal(std::stoi(m_programInput));
                m_programInput = "";
                if (m_sync.iterations != -1)
                {
                    m_sync.iterations = 0;
                }
                m_sync.waitingOnInputCV.notify_one();
            }
        }
        ImGui::End();
    }

    // Input Char Box --------------------------------------------------------------------------------------------
    if (m_pRuntime->IsWaitingForCharInput())
    {
        if (ImGui::Begin("Input Char"))
        {
            ImGui::InputText("##charInput", &m_programInput, ImGuiInputTextFlags_CallbackEdit);
            if (ImGui::Button("Submit (will only submit first character)"))
            {
                m_pRuntime->InputChar((char)m_programInput[0]);
                m_programInput = "";
                if (m_sync.iterations != -1)
                {
                    m_sync.iterations = 0;
                }
                m_sync.waitingOnInputCV.notify_one();
            }
            if (ImGui::Button("Submit Enter Char"))
            {
                m_programInput = "";
                m_pRuntime->InputChar(10);
                if (m_sync.iterations != -1)
                {
                    m_sync.iterations = 0;
                }
                m_sync.waitingOnInputCV.notify_one();
            }
        }
        ImGui::End();
    }

    m_pRuntime->RenderWindows();

    // CODE EDITOR --------------------------------------------------------------------------------------------------
    bool code_editor_open = true;
    if (ImGui::Begin("Code Editor", &code_editor_open, m_fileDialogOnTop))
    {
        ImGui::Text("Warning: Pressing ESC will delete all non submitted changes");

        if (ImGui::InputTextMultiline("##code", &m_code, ImVec2(-FLT_MIN, ImGui::GetContentRegionAvail().y - (ImGui::GetTextLineHeight() * 1.5f)), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackEdit, TextInputCallback, &m_bCodeChangedSinceLastStep))
        {
            std::cout << "code changed" << std::endl;
            m_sync.iterations = 0; // stop execution on code change
        }

        if (ImGui::Button("Run"))
        {
            m_pRuntime->Reset();
            m_pRuntime->SetSourceCode(m_code);
            m_sync.iterations = -1; // add a run speed
        }

        ImGui::SameLine();
        {
            int currentinstructionWaitTime = m_sync.instructionWaitTime.load();
            float newInstructionWaitTime = currentinstructionWaitTime / 1000.f;
            ImGui::SliderFloat("##ExecutionSpeed", &newInstructionWaitTime, 0, 3);
            m_sync.instructionWaitTime.compare_exchange_strong(currentinstructionWaitTime, static_cast<int>(newInstructionWaitTime * 1000.f));
        }

        if (m_sync.iterations == -1)
        {
            ImGui::SameLine();
            if (ImGui::Button("Pause"))
            {
                m_sync.iterations = 0;
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Step"))
        {
            ++m_sync.iterations;
        }

        if (m_sync.iterations < 0)
        {
            ImGui::SameLine();
            if (ImGui::Button("Reset"))
            {
                m_pRuntime->RequestReset();
            }
        }
    }
    ImGui::End();

    // INSTRUCTION HISTORY -----------------------------------------------------------------------------------------------
    bool displayInstructionHistory = true;
    {
        if (ImGui::Begin("Execution History", &displayInstructionHistory, m_fileDialogOnTop))
        {
            ImGui::TextWrapped(m_cachedExecutionHistory.c_str());
            ImGui::End();
        }
    }

    // PROGRAM OUTPUT ----------------------------------------------------------------------------------------------------
    bool outputDisplayOpen = true;
    if (ImGui::Begin("Program Output:", &outputDisplayOpen, m_fileDialogOnTop))
    {
        ImGui::TextWrapped(m_cachedOutput.c_str());
    }
    ImGui::End();
}

//////////////////////////////////////////////////////////////
bool CEsoProg::UpdateRuntime()
{
    if (m_bCodeChangedSinceLastStep)
    {
        m_pRuntime->SetSourceCode(m_code);
        m_bCodeChangedSinceLastStep = false;
    }
    return m_pRuntime->StepExecution();
}

//////////////////////////////////////////////////////////////
bool CEsoProg::IsRuntimeWaitingOnInput()
{
    return m_pRuntime->IsWaitingForCharInput() || m_pRuntime->IsWaitingForValInput();
}

//////////////////////////////////////////////////////////////
void CEsoProg::CopyState()
{
    m_sync.renderWantsState = true;
    m_sync.runtimeStateMtx.lock();

    m_cachedExecutionHistory = m_executionHistoryStream.str();
    m_cachedOutput = m_outputStream.str();
    m_pRuntime->CacheState();

    m_sync.renderWantsState = false;
    m_sync.runtimeStateMtx.unlock();
    m_sync.finishedStateWithCv.notify_one();
}

//////////////////////////////////////////////////////////////
void CEsoProg::Reset()
{
    m_pRuntime->Reset();
}

//////////////////////////////////////////////////////////////
void CEsoProg::CreateMenuBar()
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

        //Language Selecation
        if (ImGui::BeginMenu("Language"))
        {
            for (int language = 0; language < static_cast<int>(ELanguages::COUNT); language++)
            {
                const ELanguages::Enum eLanguage = static_cast<ELanguages::Enum>(language);
                if (ImGui::MenuItem(ELanguages::ToString(eLanguage)))
                {
                    SetCurrentLanugage(eLanguage);
                }
            }
            ImGui::EndMenu();
        }

        //Sytle
        if (ImGui::BeginMenu("Sytle"))
        {
            if (ImGui::MenuItem("Dark Mode"))
            {
                SetImGuiDarkStyle();
            }
            if (ImGui::MenuItem("Light Mode"))
            {
                SetImGuiLightStyle();
            }
            ImGui::EndMenu();
        }
    }
    ImGui::EndMainMenuBar();
}

//////////////////////////////////////////////////////////////
void CEsoProg::SetCurrentLanugage(ELanguages::Enum language)
{
    std::string newName = s_ProgramName;
    m_currentLanguage = language;
    switch (m_currentLanguage)
    {
    case (ELanguages::Piet):
    {
        m_pRuntime = &m_pietRuntime;
        newName += " - Piet";
        break;
    }
    case (ELanguages::Brainfck):
    {
        m_pRuntime = &m_brainFckRuntime;
        newName += " - Brain F*ck";
        break;
    }
    default:
    {
        m_pRuntime = &m_nullRuntime;
    }
    }
    m_pRuntime->RequestReset();
    CFileDialogBox::Set_Allowed_Type(m_pRuntime->GetSupportedFileTypes());
    glfwSetWindowTitle(s_pWindow, newName.c_str());
}

//////////////////////////////////////////////////////////////
void CEsoProg::CheckShortCuts()
{
    const ImGuiIO& io = ImGui::GetIO();
    //if a shortcut has just been used and either ctrl or the alpha key is up then enable the triggering of a new shortcut
    if (m_bShortcutUsed && (!io.KeyCtrl || !io.KeysData[m_lastShortcutUsed].Down))
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
        if (io.KeysData[ImGuiKey_N].Down)
        {
            m_bShortcutUsed = true;
            m_lastShortcutUsed = ImGuiKey_N;
            HandleNew();
        }
        else if (io.KeysData[ImGuiKey_O].Down)
        {
            m_bShortcutUsed = true;
            m_lastShortcutUsed = ImGuiKey_O;
            HandleOpen();
        }
        else if (io.KeyShift && io.KeysData[ImGuiKey_S].Down)
        {
            m_bShortcutUsed = true;
            m_lastShortcutUsed = ImGuiKey_S;
            HandelSaveAs();
        }
        else if (io.KeysData[ImGuiKey_S].Down)
        {
            m_bShortcutUsed = true;
            m_lastShortcutUsed = ImGuiKey_S;
            HandleSave();
        }
    }
}

//////////////////////////////////////////////////////////////
void CEsoProg::HandleNew()
{
    m_bFileIsNew = true;
    m_code = "";
}

//////////////////////////////////////////////////////////////
void CEsoProg::HandleOpen()
{
    m_bEnableFileDialog = true;
    m_dialogType = CFileDialogBox::FileDialogType::Open;
}

//////////////////////////////////////////////////////////////
void CEsoProg::HandleSave()
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

//////////////////////////////////////////////////////////////
void CEsoProg::HandelSaveAs()
{
    m_bEnableFileDialog = true;
    m_dialogType = CFileDialogBox::FileDialogType::Save_As;
}

//////////////////////////////////////////////////////////////
void CEsoProg::PreFileLoad(const std::filesystem::path path)
{
    const EFileType::Enum fileType = path.extension() == ".txt" ? EFileType::Text : EFileType::Image;
    switch (fileType)
    {
    case EFileType::Image:
    {
        if (m_currentFileType == EFileType::Image)
        {
            if (m_pRuntime->GetRuntimeLanguage() == ELanguages::Piet)
            {
                static_cast<PietRuntime*>(m_pRuntime)->UnsetImage();
                if (m_bImageLoaded)
                {
                    stbi_image_free(m_imageData);
                    m_bImageLoaded = false;
                }
            }
        }
        break;
    }
    case EFileType::Text:
    {
        if (m_currentFileType == EFileType::Text)
        {
            m_code = "";
            m_bCodeChangedSinceLastStep = true;
        }
    }
    }
    m_outputStream.str(std::string());
    m_executionHistoryStream.str(std::string());
    m_pRuntime->RequestReset();
}

//////////////////////////////////////////////////////////////
CEsoProg::EFileType::Enum CEsoProg::LoadFile(const std::filesystem::path path)
{
    const EFileType::Enum fileType = path.extension() == ".txt" ? EFileType::Text : EFileType::Image;
    PreFileLoad(path);
    switch (fileType)
    {
    case EFileType::Text:
    {
        m_fileStream.open(path.string(), m_fileStream.in);
        m_code = "";
        std::getline(m_fileStream, m_code, char(0));
        m_bFileIsNew = false;
        m_currentFilePath = path;
        m_fileStream.close();
        m_pRuntime->SetSourceCode(m_code);
        break;
    }
    case EFileType::Image:
    {
        if (m_pRuntime->GetRuntimeLanguage() == ELanguages::Piet)
        {

            if (m_bImageLoaded)
            {
                stbi_image_free(m_imageData);
            }

            m_imageData = stbi_load(path.string().c_str(), &m_imageWidth, &m_imageHeight, &m_NumComponents, 4);
            if (m_imageData)
            {
                glBindTexture(GL_TEXTURE_2D, m_texture);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
                //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
                glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_imageWidth, m_imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_imageData);
                //glGenerateMipmap(GL_TEXTURE_2D);
                m_bImageLoaded = true;
            }
            m_sync.renderWantsState = true;
            m_sync.runtimeStateMtx.lock();

            static_cast<PietRuntime*>(m_pRuntime)->SetImage(&m_texture, m_imageData, m_imageWidth, m_imageHeight);
            m_sync.renderWantsState = false;
            m_sync.runtimeStateMtx.unlock();
            m_sync.finishedStateWithCv.notify_one();
        }
        break;
    }
    }
    PostFileLoad(fileType, path);
    return fileType;
}

//////////////////////////////////////////////////////////////
void CEsoProg::PostFileLoad(const CEsoProg::EFileType::Enum fileType, const std::filesystem::path path)
{
    m_currentFileType = fileType;
}
