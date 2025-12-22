#include "ImGuiValueChangeCallbacks.h"

#include <imgui.h>  // for ImGuiInputTextCallbackData

#include <string>

//////////////////////////////////////////////////////////////
int TextInputCallback(ImGuiInputTextCallbackData* pData)
{
    *((bool*)pData->UserData) = true;

    //don't change the input key
    return 0;
}

//////////////////////////////////////////////////////////////
int ValueInputChanged(ImGuiInputTextCallbackData* pData)
{
    if (std::string str = pData->Buf; !str.empty())
    {
        *((int*)pData->UserData) = std::stoi(str);
    }
    return 0;
}
