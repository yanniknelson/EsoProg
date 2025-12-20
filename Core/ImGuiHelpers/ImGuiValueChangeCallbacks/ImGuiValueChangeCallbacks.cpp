#include "ImGuiValueChangeCallbacks.h"

#include <imgui.h>  // for ImGuiInputTextCallbackData

#include <string>

int TextInputCallback(ImGuiInputTextCallbackData* data)
{
    *((bool*)data->UserData) = true;

    //don't change the input key
    return 0;
}

int ValueInputChanged(ImGuiInputTextCallbackData* data)
{
    if (std::string str = data->Buf; !str.empty())
    {
        *((int*)data->UserData) = std::stoi(str);
    }
    return 0;
}
