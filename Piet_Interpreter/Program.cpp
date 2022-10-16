#include "Program.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Resources/stb_image.h"

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

Program::FileType Program::OpenFile(std::filesystem::path path) {
    if (path.extension() == ".txt") {
        fs.open(path.string(), fs.in);
        code = "";
        std::getline(fs, code, char(0));
        file_is_new = false;
        currentFilePath = path;
        fs.close();
        return FileType::Text;
    } else {
        if (image_loaded) {
            stbi_image_free(image_data);
        }
        image_data = stbi_load(path.string().c_str(), &image_width, &image_height, &num_components, 4);
        if (image_data) {
            aspect_ratio = (float)image_height / (float)image_width;
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
            glGenerateMipmap(GL_TEXTURE_2D);
            image_loaded = true;
        }
        return FileType::Image;
    }
}

void Program::Render() {
    file_dialog_on_top = ImGuiWindowFlags_None;
    //check if any shortcut is active and handle it
    Check_ShortCuts();

    CreateMenuBar();

    // FILE DIALOG BOX ------------------------------------------------------------------------------------------------------
    if (enable_file_dialog) {
        //set the nobring to front on focus flag for all other elements to ensure
        //the file dialog is always on top
        file_dialog_on_top = ImGuiWindowFlags_NoBringToFrontOnFocus;
        FileDialogBox::FileDialogReturn ret = FileDialogBox::Create_File_Dialog(enable_file_dialog, dialogType);
        if (ret.selected) {
            switch (dialogType)
            {
            case FileDialogBox::Open:
                OpenFile(ret.path);
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

    // CODE EDITOR --------------------------------------------------------------------------------------------------
    bool code_editor_open = true;
    if (ImGui::Begin("Code Editor", &code_editor_open, file_dialog_on_top)) {
        ImGui::Text("Warning: Pressing ESC will delete all non submitted changes");
        bool enterPressed = false;
        if (ImGui::InputTextMultiline("##code", &code, ImVec2(-FLT_MIN, ImGui::GetContentRegionAvail().y - (ImGui::GetTextLineHeight() * 1.5)), ImGuiInputTextFlags_CallbackCharFilter | ImGuiInputTextFlags_EnterReturnsTrue, Check_For_Enter, &enterPressed)) {
            std::cout << code << std::endl;
        }
        if (enterPressed) {

        }
        if (ImGui::Button("Compile")) {
            is_token_error = false;
            std::istrstream input(code.c_str());
            tk.set_stream(input);
            Tokeniser::Token& token = tk.pop();
            while (token.kind != Tokeniser::Kind::End) {
                std::cout << tk.get_line_number() << " " << token << std::endl;
                token = tk.pop();
                if (token.kind == Tokeniser::Kind::Unrecognised_Token) {
                    is_token_error = true;
                    token_error_line = tk.get_line_number();
                    break;
                }
            }
            if (!is_token_error) {
                runtime.set_stream(code);
                is_compiled = true;
            }
        }

        if (is_compiled) {
            if (is_token_error) {
                ImGui::SameLine();
                ImGui::Text("There's an error on line %d, lower lines have not been checked", token_error_line);
            }
            else {
                ImGui::SameLine();
                if (ImGui::Button("Run")) {
                    output = "";
                    runtime.run(); // add a run speed
                }
                ImGui::SameLine();
                if (ImGui::Button("Pause")) {

                }
                ImGui::SameLine();
                if (ImGui::Button("Step")) {
                    runtime.step_execution();
                }
            }
        }
        ImGui::End();
    }

    // IMAGE PROGRAM DISPLAY --------------------------------------------------------------------------------------------
    if (ImGui::Begin("OpenGL Texture Text")) {
        if (image_loaded) {
            ImVec2 area = ImGui::GetContentRegionAvail();
            ImVec2 desired = ImVec2(area.x, (int)area.x * aspect_ratio);
            if (desired.y > area.y) {
                desired = ImVec2((int)area.y * (1/aspect_ratio), area.y);
            }
            ImGui::Image((void*)(intptr_t)texture, desired);
        }
        ImGui::End();
    }

    // STACK DISPLAY ----------------------------------------------------------------------------------------------------
    bool stack_display_open = true;
    if (ImGui::Begin("Stack", &stack_display_open, file_dialog_on_top)) {
        ImGui::Text("The stack is displayed with the deepest value at the top");
        if (ImGui::BeginListBox("##Stack", ImGui::GetContentRegionAvail())) {
           const std::deque<int> stack = runtime.getStack();
           for (int i = stack.size()-1; i >= 0 ; i--) {
               std::string lbl = std::to_string(stack[i]) + "##" + std::to_string(i);
               ImGui::Selectable(lbl.c_str(), false, ImGuiSelectableFlags_Disabled);
           }
           ImGui::EndListBox();
        }
        ImGui::End();
    }

    // PROGRAM OUTPUT ----------------------------------------------------------------------------------------------------
    bool output_display_open = true;
    if (ImGui::Begin("Program Output", &output_display_open, file_dialog_on_top)) {
        ImGui::Text("The stack is displayed with the deepest value at the top");
        ImGui::TextWrapped(output.c_str());
        ImGui::End();
    }
}