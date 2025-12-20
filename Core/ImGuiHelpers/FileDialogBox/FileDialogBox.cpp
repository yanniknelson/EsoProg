#include "FileDialogBox.h"

#include <IconsFontAwesome7.h>
#include <imgui.h>
#include <imgui_stdlib.h>

const char* FileDialogBox::DialogBoxTypes[] = { "Open", "Save As" };
fs::path FileDialogBox::currentPath = fs::path();
fs::path FileDialogBox::selectedFilePath = fs::path();
std::string FileDialogBox::currentFileName = "";
std::vector<std::string> FileDialogBox::allowed_types = {};

FileDialogBox::FileDialogReturn FileDialogBox::Create_File_Dialog(bool& open, FileDialogBox::FileDialogType type)
{
    bool submitted = false;
    ImGui::Begin("File Explorer", &open, ImGuiWindowFlags_NoCollapse); //| ImGuiWindowFlags_NoDocking);
    //decompose the current Path
    fs::path p = currentPath;
    std::vector<fs::path> breakdown = {};
    while (p != p.root_path())
    {
        breakdown.push_back(p);
        p = p.parent_path();
    }
    //in reverse of the decomposed path, create buttons pointing the the file of each decomposition
    if (ImGui::Button(p.root_name().string().c_str()))
    {
        currentPath = p.root_path();
    }
    for (auto pth = breakdown.rbegin(); pth != breakdown.rend(); pth++)
    {
        ImGui::SameLine();
        if (ImGui::Button((pth->filename().string() + "##" + pth->string()).c_str()))
        {
            currentPath = *pth;
        }
    }
    //create a button for each entry in the current directory with the text on the button being the entry's type + name
    if (ImGui::BeginChild("Files", ImVec2(0, ImGui::GetContentRegionAvail().y - (ImGui::GetTextLineHeight() * 2)), true))
    {
        if (ImGui::Button(".."))
        {
            currentPath = currentPath.parent_path();
        }
        for (const fs::directory_entry& entry : fs::directory_iterator(currentPath))
        {
            //filter files for text files only
            if (!entry.is_directory() && std::find(allowed_types.begin(), allowed_types.end(), entry.path().extension()) == allowed_types.end())
            {
                continue;
            }

            std::string buttonName;
            if (entry.is_directory())
            {
                buttonName = ICON_FA_FOLDER;
            }
            else
            {
                buttonName = ICON_FA_FILE;
            }

            buttonName += " " + entry.path().filename().string();
            if (ImGui::Button(buttonName.c_str()))
            {
                //if the entry is a directory, then when we click it update the current path
                if (entry.is_directory())
                {
                    currentPath = entry.path();
                }
                else
                {
                    selectedFilePath = entry.path();
                    currentFileName = selectedFilePath.filename().string();
                }
            }
        }
        ImGui::EndChild();
    }

    if (type == FileDialogType::Save_As)
    {
        ImGui::InputText("##file_name", &currentFileName);
    }
    else
    {
        ImGui::Text(selectedFilePath.filename().string().c_str());
    }

    ImGui::SameLine();
    if (ImGui::Button(DialogBoxTypes[type]) && (type != FileDialogType::Save_As || !currentFileName.empty()))
    {
        submitted = true;
        open = false;
    }

    ImGui::End();
    if (type != FileDialogType::Save_As)
    {
        return { submitted && !selectedFilePath.empty(), selectedFilePath };
    }
    else
    {
        fs::path save_as_path = currentPath / (currentFileName);
        return { submitted, save_as_path.replace_extension(".txt") };
    }
}
