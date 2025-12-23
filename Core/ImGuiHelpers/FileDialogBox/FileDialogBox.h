#pragma once

#include <filesystem>  // for std::filesystem::path
#include <string>
#include <vector>

namespace fs = std::filesystem;

//////////////////////////////////////////////////////////////
class CFileDialogBox
{
  public:
    //////////////////////////////////////////////////////////////
    struct SFileDialogReturn
    {
        bool m_bSelected = false;
        fs::path m_path;
    };

    static void Init_Path(fs::path path);
    static void Set_Allowed_Type(std::vector<std::string> types);
    static void Add_Allowed_Type(std::string type);

    enum FileDialogType
    {
        Open,
        Save_As,
        Num_of_Types
    };

    static SFileDialogReturn Create_File_Dialog(bool& rbOpen, FileDialogType type);

  private:
    static const char* s_dialogBoxTypes[];
    static fs::path s_currentPath;
    static fs::path s_selectedFilePath;
    static std::string s_currentFileName;
    static std::vector<std::string> s_allowed_types;
};
