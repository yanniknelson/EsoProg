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
        bool selected = false;
        fs::path path;
    };

    static void Init_Path(fs::path p);
    static void Set_Allowed_Type(std::vector<std::string> types);
    static void Add_Allowed_Type(std::string type);

    enum FileDialogType
    {
        Open,
        Save_As,
        Num_of_Types
    };

    static SFileDialogReturn Create_File_Dialog(bool& open, FileDialogType type);

  private:
    static const char* DialogBoxTypes[];
    static fs::path currentPath;
    static fs::path selectedFilePath;
    static std::string currentFileName;
    static std::vector<std::string> allowed_types;
};
