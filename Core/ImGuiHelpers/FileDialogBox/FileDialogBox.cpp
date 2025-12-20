#include "FileDialogBox.h"

#include <IconsFontAwesome7.h>  // for ICON_FA_FOLDER, ICON_FA_FILE
#include <imgui.h>              // for all imgui functions
#include <imgui_stdlib.h>       // for InputText and others with std::string input

#include <algorithm>            // for std::find
#include <filesystem>           // for std::filesystem::path
#include <iostream>
#include <string>
#include <vector>

const char* CFileDialogBox::s_dialogBoxTypes[] = { "Open", "Save As" };
fs::path CFileDialogBox::s_currentPath = fs::path();
fs::path CFileDialogBox::s_selectedFilePath = fs::path();
std::string CFileDialogBox::s_currentFileName = "";
std::vector<std::string> CFileDialogBox::s_allowed_types = {};

//////////////////////////////////////////////////////////////
void CFileDialogBox::Init_Path(fs::path path)
{
    s_currentPath = path;
    std::cout << s_currentPath.string() << "\n";
}

//////////////////////////////////////////////////////////////
void CFileDialogBox::Set_Allowed_Type(std::vector<std::string> types)
{
    s_allowed_types = types;
}

//////////////////////////////////////////////////////////////
void CFileDialogBox::Add_Allowed_Type(std::string type)
{
    s_allowed_types.push_back(type);
}

//////////////////////////////////////////////////////////////
CFileDialogBox::SFileDialogReturn CFileDialogBox::Create_File_Dialog(bool& rbOpen, CFileDialogBox::FileDialogType type)
{
    bool bSubmitted = false;
    ImGui::Begin("File Explorer", &rbOpen, ImGuiWindowFlags_NoCollapse); //| ImGuiWindowFlags_NoDocking);
    //decompose the current Path
    fs::path workingPath = s_currentPath;
    std::vector<fs::path> breakdown = {};
    while (workingPath != workingPath.root_path())
    {
        breakdown.push_back(workingPath);
        workingPath = workingPath.parent_path();
    }

    //in reverse of the decomposed path, create buttons pointing the the file of each decomposition
    if (ImGui::Button(workingPath.root_name().string().c_str()))
    {
        s_currentPath = workingPath.root_path();
    }

    for (auto itSubpath = breakdown.rbegin(); itSubpath != breakdown.rend(); itSubpath++)
    {
        ImGui::SameLine();
        if (ImGui::Button((itSubpath->filename().string() + "##" + itSubpath->string()).c_str()))
        {
            s_currentPath = *itSubpath;
        }
    }

    //create a button for each entry in the current directory with the text on the button being the entry's type + name
    if (ImGui::BeginChild("Files", ImVec2(0, ImGui::GetContentRegionAvail().y - (ImGui::GetTextLineHeight() * 2)), true))
    {
        if (ImGui::Button(".."))
        {
            s_currentPath = s_currentPath.parent_path();
        }

        for (const fs::directory_entry& rEntry : fs::directory_iterator(s_currentPath))
        {
            //filter files for text files only
            if (!rEntry.is_directory() && std::find(s_allowed_types.begin(), s_allowed_types.end(), rEntry.path().extension()) == s_allowed_types.end())
            {
                continue;
            }

            std::string buttonName;
            if (rEntry.is_directory())
            {
                buttonName = ICON_FA_FOLDER;
            }
            else
            {
                buttonName = ICON_FA_FILE;
            }

            buttonName += " " + rEntry.path().filename().string();
            if (ImGui::Button(buttonName.c_str()))
            {
                //if the entry is a directory, then when we click it update the current path
                if (rEntry.is_directory())
                {
                    s_currentPath = rEntry.path();
                }
                else
                {
                    s_selectedFilePath = rEntry.path();
                    s_currentFileName = s_selectedFilePath.filename().string();
                }
            }
        }
        ImGui::EndChild();
    }

    if (type == FileDialogType::Save_As)
    {
        ImGui::InputText("##file_name", &s_currentFileName);
    }
    else
    {
        ImGui::Text(s_selectedFilePath.filename().string().c_str());
    }

    ImGui::SameLine();
    if (ImGui::Button(s_dialogBoxTypes[type]) && (type != FileDialogType::Save_As || !s_currentFileName.empty()))
    {
        bSubmitted = true;
        rbOpen = false;
    }

    ImGui::End();
    if (type != FileDialogType::Save_As)
    {
        return { bSubmitted && !s_selectedFilePath.empty(), s_selectedFilePath };
    }
    else
    {
        fs::path save_as_path = s_currentPath / (s_currentFileName);
        return { bSubmitted, save_as_path.replace_extension(".txt") };
    }
}
