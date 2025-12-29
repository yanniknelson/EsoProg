#pragma once

#include <BrainFckRuntime.h>    // for BrainFckRuntime
#include <ELanguages.h>         // for ELanguages::Enum
#include <FileDialogBox.h>      // for CFileDialogBox::FileDialogType, CFileDialogBox::Init_Path
#include <IRuntime.h>           // for IRuntime
#include <LogManager.h>         // for TLoggerPtr
#include <NullRuntime.h>        // for NullRuntime
#include <PietRuntime.h>        // for PietRuntime
#include <SmartEnums.h>         // for CreateSmartEnum

#include <GLFW/glfw3.h>         // for GLFWwindow
#include <gl/GL.h>              // for GLuint
#include <imgui.h>              // for ImGuiKey, ImGuiWindowFlags_, ImGuiInputTextFlags, ImGuiIO, ImWchar, ImFontConfig

#include <filesystem>           // for std::filesystem::path
#include <fstream>
#include <sstream>              // for std::ostringstream
#include <string>

//////////////////////////////////////////////////////////////
class CEsoProg
{
  public:
    static const char* s_programName;
    static GLFWwindow* s_pWindow;

    SRuntimeSyncronisationStruct m_sync;

    CEsoProg(GLFWwindow* pWindow);

    TLoggerPtr GetLogger();
    void Render();
    bool UpdateRuntime();
    bool IsRuntimeWaitingOnInput();
    void CopyState();
    void Reset();

  private:
    // clang-format off
#define EFILETYPES(x) \
    x(Text)           \
    x(Image)

    CreateSmartEnum(EFileType, EFILETYPES);

#undef EFILETYPES
    // clang-format on

    void CreateMenuBar();

    void SetCurrentLanugage(ELanguages::Enum language);

    void CheckShortCuts();

    void HandleNew();
    void HandleOpen();
    void HandelSaveAs();
    void HandleSave();

    void PreFileLoad(const std::filesystem::path path);
    EFileType::Enum LoadFile(const std::filesystem::path path);
    void PostFileLoad(const EFileType::Enum fileType, const std::filesystem::path path);

    TLoggerPtr m_pLogger{ nullptr };

    //shortcut flags
    bool m_bShortcutUsed = false;
    ImGuiKey m_lastShortcutUsed = ImGuiKey_None;

    //file dialog box flags
    bool m_bEnableFileDialog = false;
    CFileDialogBox::FileDialogType m_dialogType = CFileDialogBox::FileDialogType::Open;
    ImGuiWindowFlags_ m_fileDialogOnTop = ImGuiWindowFlags_None;
    std::fstream m_fileStream;

    EFileType::Enum m_currentFileType = EFileType::COUNT;

    ELanguages::Enum m_currentLanguage = ELanguages::COUNT;

    //file management
    bool m_bFileIsNew = true;
    fs::path m_currentFilePath = "";

    //Image
    unsigned char* m_pImageData{ nullptr };
    int m_imageWidth = 0;
    int m_imageHeight = 0;
    int m_numComponents = 0;
    bool m_bImageLoaded = false;
    float m_aspectRatio = 1.f;

    GLuint m_texture;

    //Interpreting
    bool m_bIsTokenError = false;
    bool m_bCodeChangedSinceLastStep = false;
    int m_tokenErrorLine = 0;
    std::ostringstream m_outputStream{ "" };
    std::ostringstream m_executionHistoryStream{ "" };
    std::string m_cachedExecutionHistory{ "" };
    std::string m_cachedOutput{ "" };

    IRuntime* m_pRuntime{ nullptr };
    NullRuntime m_nullRuntime{ m_sync, m_outputStream, m_executionHistoryStream };
    PietRuntime m_pietRuntime{ m_sync, m_outputStream, m_executionHistoryStream };
    BrainFckRuntime m_brainFckRuntime{ m_sync, m_outputStream, m_executionHistoryStream };

    ImGuiInputTextFlags m_codeEditorFlags = ImGuiInputTextFlags_CallbackCharFilter | ImGuiInputTextFlags_EnterReturnsTrue;
    std::string m_code{ "" };
    std::string m_programInput{ "" };
};
