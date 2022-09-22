#pragma once

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

class FileDialogBox {

    static fs::path currentPath;
    static fs::path selectedFilePath;
    static std::string currentFileName;

public:

    struct FileDialogReturn {
        bool selected = false;
        fs::path path;
    };

    static void Init_Path(fs::path p) {
        currentPath = p;
        std::cout << currentPath.string() << "\n";
    }
    
    enum FileDialogType {
        Open,
        Save_As,
        Num_of_Types
    };

    static const char* DialogBoxTypes[];

    static FileDialogReturn Create_File_Dialog(bool& open, FileDialogType type);

};