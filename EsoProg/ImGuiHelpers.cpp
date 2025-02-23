#include "ImGuiHelpers.h"

int TextInputCallback(ImGuiInputTextCallbackData* data)
{
	*((bool*)data->UserData) = false;

	//don't change the input key
	return 0;
}

int ValueInputChanged(ImGuiInputTextCallbackData* data)
{
	*((int*)data->UserData) = std::stoi(data->Buf);

	return 0;
}