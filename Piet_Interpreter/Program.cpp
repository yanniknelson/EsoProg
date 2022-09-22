#include "Program.h"

int Program::Check_For_Enter(ImGuiInputTextCallbackData* data)
{
    //if the key pressed was enter (catching windows and linux new lines) set the userdata flag to true
    if (data->EventChar == 10 || data->EventChar == 13) {
        *((bool *) data->UserData) = true;
    }
    //don't change the input key
    return 0;
}

void Program::handle_New() {
    std::cout << "New\n";
    file_is_new = true;
    code = "";
}

void Program::handle_Open() {
    std::cout << "Open\n";
    enable_file_dialog = true;
    dialogType = FileDialogBox::FileDialogType::Open;
}

void Program::handle_Save() {
    if (file_is_new) {
        handle_Save_As();
    }
    else {
        std::cout << "Save\n";
        std::ofstream fs;
        fs.open(currentFilePath.string());
        fs << code;
        fs.close();
    }
}

void Program::handle_Save_As() {
    std::cout << "Save As\n";
    enable_file_dialog = true;
    dialogType = FileDialogBox::FileDialogType::Save_As;
}

void Program::Check_ShortCuts() {
    const ImGuiIO& io = ImGui::GetIO();
    //if a shortcut has just been used and either ctrl or the alpha key is up then enable the triggering of a new shortcut
    if (shortcut_used && (!io.KeyCtrl || !io.KeysDown[last_shortcut_used])) {
        shortcut_used = false;
    }

    //if we have dissabled the triggering of a new shortcut return false
    if (shortcut_used) { return; }

    //check the combo for a shortcut, disable new shortcuts, store the used alpha key and handle the shortcut
    if (io.KeyCtrl) {
        if (io.KeysDown[ImGuiKey_N]) {
            shortcut_used = true;
            last_shortcut_used = ImGuiKey_N;
            handle_New();
        }
        else if (io.KeysDown[ImGuiKey_O]) {
            shortcut_used = true;
            last_shortcut_used = ImGuiKey_O;
            handle_Open();
        }
        else if (io.KeyShift && io.KeysDown[ImGuiKey_S]) {
            shortcut_used = true;
            last_shortcut_used = ImGuiKey_S;
            handle_Save_As();
        } 
        else if (io.KeysDown[ImGuiKey_S]) {
            shortcut_used = true;
            last_shortcut_used = ImGuiKey_S;
            handle_Save();
        } 
    }

}

void Program::CreateMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        //file menu
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem(ICON_FA_FILE_CIRCLE_PLUS " New", "Ctrl+N")) {
                handle_New();
            }
            if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN " Open", "Ctrl+O")) {
                handle_Open();
            }
            if (ImGui::MenuItem(ICON_FA_FLOPPY_DISK " Save", "Ctrl+S")) {
                handle_Save();
            }
            if (ImGui::MenuItem(ICON_FA_FLOPPY_DISK " Save As", "Ctrl+Shift+S")) {
                handle_Save_As();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void Program::Render() {
    file_dialog_on_top = ImGuiWindowFlags_None;
    //check if any shortcut is active and handle it
    Check_ShortCuts();

    CreateMenuBar();

    if (enable_file_dialog) {
        //set the nobring to front on focus flag for all other elements to ensure
        //the file dialog is always on top
        file_dialog_on_top = ImGuiWindowFlags_NoBringToFrontOnFocus;
        FileDialogBox::FileDialogReturn ret = FileDialogBox::Create_File_Dialog(enable_file_dialog, dialogType);
        if (ret.selected) {
            std::fstream fs;
            switch (dialogType)
            {
            case FileDialogBox::Open:
                fs.open(ret.path.string(), fs.in);
                code = "";
                std::getline(fs, code, char(0));
                file_is_new = false;
                currentFilePath = ret.path;
                fs.close();
                break;
            case FileDialogBox::Save_As:
                fs.open(ret.path.string(), fs.out);
                fs << code;
                fs.close();
                file_is_new = false;
                currentFilePath = ret.path;
                break;
            default:
                std::cout << "default" << std::endl;
                break;
            }
        }
    }

    bool code_editor_open = true;
    ImGui::Begin("Code Editor", &code_editor_open, file_dialog_on_top);
    ImGui::Text("Warning: Pressing ESC will delete all non submitted changes");
    bool enterPressed = false;
    if (ImGui::InputTextMultiline("##code", &code, ImVec2(-FLT_MIN, ImGui::GetContentRegionAvail().y - (ImGui::GetTextLineHeight() * 1.5)), ImGuiInputTextFlags_CallbackCharFilter | ImGuiInputTextFlags_EnterReturnsTrue, Check_For_Enter, &enterPressed)) {
        std::cout << code << std::endl;
    }
    if (enterPressed) {
        std::cout << "bool changed" << std::endl;
    }
    ImGui::Button("Run");
    ImGui::End();
}