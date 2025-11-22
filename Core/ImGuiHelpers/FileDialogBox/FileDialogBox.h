#pragma once

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

class FileDialogBox {

    static fs::path currentPath;
    static fs::path selectedFilePath;
    static std::string currentFileName;
    static std::vector<std::string> allowed_types;

public:

    struct FileDialogReturn {
        bool selected = false;
        fs::path path;
    };

    static void Init_Path(fs::path p) {
        currentPath = p;
        std::cout << currentPath.string() << "\n";
    }

    static void Set_Allowed_Type(std::vector<std::string> types) {
        allowed_types = types;
    }

    static void Add_Allowed_Type(std::string type) {
        allowed_types.push_back(type);
    }

    enum FileDialogType {
        Open,
        Save_As,
        Num_of_Types
    };

    static const char* DialogBoxTypes[];

    static FileDialogReturn Create_File_Dialog(bool& open, FileDialogType type);

};