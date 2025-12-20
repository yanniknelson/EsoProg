#pragma once

#include "ImGuiSetStyles.h"

#include <imgui.h>  // for ImGuiStyle, ImGui::GetStyle, ImVec2, ImGuiDir_..., ImGuiCol_....

//////////////////////////////////////////////////////////////
void SetImGuiShapingStyle()
{
    ImGuiStyle& rStyle = ImGui::GetStyle();

    rStyle.Alpha = 1.0f;
    rStyle.DisabledAlpha = 1.0f;
    rStyle.WindowPadding = ImVec2(12.0f, 12.0f);
    rStyle.WindowRounding = 11.5f;
    rStyle.WindowBorderSize = 1.0f;
    rStyle.WindowMinSize = ImVec2(20.0f, 20.0f);
    rStyle.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    rStyle.WindowMenuButtonPosition = ImGuiDir_Left;
    rStyle.ChildRounding = 3.0f;
    rStyle.ChildBorderSize = 1.0f;
    rStyle.PopupRounding = 11.5f;
    rStyle.PopupBorderSize = 1.0f;
    rStyle.FramePadding = ImVec2(5.0f, 3.4f);
    rStyle.FrameRounding = 11.9f;
    rStyle.FrameBorderSize = 0.0f;
    rStyle.ItemSpacing = ImVec2(4.3f, 5.5f);
    rStyle.ItemInnerSpacing = ImVec2(7.1f, 1.8f);
    rStyle.CellPadding = ImVec2(12.1f, 9.2f);
    rStyle.IndentSpacing = 0.0f;
    rStyle.ColumnsMinSpacing = 4.9f;
    rStyle.ScrollbarSize = 11.6f;
    rStyle.ScrollbarRounding = 15.9f;
    rStyle.GrabMinSize = 3.7f;
    rStyle.GrabRounding = 20.0f;
    rStyle.TabRounding = 10.0f;
    rStyle.TabBorderSize = 1.0f;
    //rStyle.TabMinWidthForCloseButton = 0.0f;
    rStyle.ColorButtonPosition = ImGuiDir_Left;
    rStyle.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    rStyle.SelectableTextAlign = ImVec2(0.0f, 0.0f);
}

//////////////////////////////////////////////////////////////
void SetImGuiDarkStyle()
{
    ImGuiStyle& rStyle = ImGui::GetStyle();

    rStyle.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    rStyle.Colors[ImGuiCol_TextDisabled] = ImVec4(0.27450982f, 0.31764707f, 0.4509804f, 1.0f);
    rStyle.Colors[ImGuiCol_WindowBg] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
    rStyle.Colors[ImGuiCol_ChildBg] = ImVec4(0.09411765f, 0.101960786f, 0.11764706f, 1.0f);
    rStyle.Colors[ImGuiCol_PopupBg] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
    rStyle.Colors[ImGuiCol_Border] = ImVec4(0.15686275f, 0.16862746f, 0.19215687f, 1.0f);
    rStyle.Colors[ImGuiCol_BorderShadow] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
    rStyle.Colors[ImGuiCol_FrameBg] = ImVec4(0.11372549f, 0.1254902f, 0.15294118f, 1.0f);
    rStyle.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15686275f, 0.16862746f, 0.19215687f, 1.0f);
    rStyle.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.15686275f, 0.16862746f, 0.19215687f, 1.0f);
    rStyle.Colors[ImGuiCol_TitleBg] = ImVec4(0.047058824f, 0.05490196f, 0.07058824f, 1.0f);
    rStyle.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.047058824f, 0.05490196f, 0.07058824f, 1.0f);
    rStyle.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
    rStyle.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803922f, 0.105882354f, 0.12156863f, 1.0f);
    rStyle.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.047058824f, 0.05490196f, 0.07058824f, 1.0f);
    rStyle.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.0f);
    rStyle.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.15686275f, 0.16862746f, 0.19215687f, 1.0f);
    rStyle.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.0f);
    rStyle.Colors[ImGuiCol_CheckMark] = ImVec4(0.03137255f, 0.9490196f, 0.84313726f, 1.0f);
    rStyle.Colors[ImGuiCol_SliderGrab] = ImVec4(0.03137255f, 0.9490196f, 0.84313726f, 1.0f);
    rStyle.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.03137255f, 0.9490196f, 0.84313726f, 1.0f);
    rStyle.Colors[ImGuiCol_Button] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.0f);
    rStyle.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.18039216f, 0.1882353f, 0.19607843f, 1.0f);
    rStyle.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15294118f, 0.15294118f, 0.15294118f, 1.0f);
    rStyle.Colors[ImGuiCol_Header] = ImVec4(0.14117648f, 0.16470589f, 0.20784314f, 1.0f);
    rStyle.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.105882354f, 0.105882354f, 0.105882354f, 1.0f);
    rStyle.Colors[ImGuiCol_HeaderActive] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
    rStyle.Colors[ImGuiCol_Separator] = ImVec4(0.12941177f, 0.14901961f, 0.19215687f, 1.0f);
    rStyle.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.15686275f, 0.18431373f, 0.2509804f, 1.0f);
    rStyle.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.15686275f, 0.18431373f, 0.2509804f, 1.0f);
    rStyle.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.14509805f, 0.14509805f, 0.14509805f, 1.0f);
    rStyle.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.03137255f, 0.9490196f, 0.84313726f, 1.0f);
    rStyle.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    rStyle.Colors[ImGuiCol_Tab] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
    rStyle.Colors[ImGuiCol_TabHovered] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.0f);
    rStyle.Colors[ImGuiCol_TabActive] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.0f);
    rStyle.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
    rStyle.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1254902f, 0.27450982f, 0.57254905f, 1.0f);
    rStyle.Colors[ImGuiCol_PlotLines] = ImVec4(0.52156866f, 0.6f, 0.7019608f, 1.0f);
    rStyle.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.039215688f, 0.98039216f, 0.98039216f, 1.0f);
    rStyle.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.03137255f, 0.9490196f, 0.84313726f, 1.0f);
    rStyle.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.15686275f, 0.18431373f, 0.2509804f, 1.0f);
    rStyle.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.047058824f, 0.05490196f, 0.07058824f, 1.0f);
    rStyle.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.047058824f, 0.05490196f, 0.07058824f, 1.0f);
    rStyle.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    rStyle.Colors[ImGuiCol_TableRowBg] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.0f);
    rStyle.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803922f, 0.105882354f, 0.12156863f, 1.0f);
    rStyle.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.9372549f, 0.9372549f, 0.9372549f, 1.0f);
    rStyle.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.49803922f, 0.5137255f, 1.0f, 1.0f);
    rStyle.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26666668f, 0.2901961f, 1.0f, 1.0f);
    rStyle.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.49803922f, 0.5137255f, 1.0f, 1.0f);
    rStyle.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.19607843f, 0.1764706f, 0.54509807f, 0.5019608f);
    rStyle.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.19607843f, 0.1764706f, 0.54509807f, 0.5019608f);
}

//////////////////////////////////////////////////////////////
void SetImGuiLightStyle()
{
    ImGuiStyle& rStyle = ImGui::GetStyle();

    rStyle.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    rStyle.Colors[ImGuiCol_TextDisabled] = ImVec4(0.7254902f, 0.68235296f, 0.54901963f, 1.0f);
    rStyle.Colors[ImGuiCol_WindowBg] = ImVec4(0.92156863f, 0.9137255f, 0.8980392f, 1.0f);
    rStyle.Colors[ImGuiCol_ChildBg] = ImVec4(0.90588236f, 0.8980392f, 0.88235295f, 1.0f);
    rStyle.Colors[ImGuiCol_PopupBg] = ImVec4(0.92156863f, 0.9137255f, 0.8980392f, 1.0f);
    rStyle.Colors[ImGuiCol_Border] = ImVec4(0.84313726f, 0.83137256f, 0.80784315f, 1.0f);
    rStyle.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92156863f, 0.9137255f, 0.8980392f, 1.0f);
    rStyle.Colors[ImGuiCol_FrameBg] = ImVec4(0.8862745f, 0.8745098f, 0.84705883f, 1.0f);
    rStyle.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.84313726f, 0.83137256f, 0.80784315f, 1.0f);
    rStyle.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.84313726f, 0.83137256f, 0.80784315f, 1.0f);
    rStyle.Colors[ImGuiCol_TitleBg] = ImVec4(0.9529412f, 0.94509804f, 0.92941177f, 1.0f);
    rStyle.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.9529412f, 0.94509804f, 0.92941177f, 1.0f);
    rStyle.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.92156863f, 0.9137255f, 0.8980392f, 1.0f);
    rStyle.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.9019608f, 0.89411765f, 0.8784314f, 1.0f);
    rStyle.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.9529412f, 0.94509804f, 0.92941177f, 1.0f);
    rStyle.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.88235295f, 0.8666667f, 0.8509804f, 1.0f);
    rStyle.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.84313726f, 0.83137256f, 0.80784315f, 1.0f);
    rStyle.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.88235295f, 0.8666667f, 0.8509804f, 1.0f);
    rStyle.Colors[ImGuiCol_CheckMark] = ImVec4(0.96862745f, 0.050980393f, 0.15686275f, 1.0f);
    rStyle.Colors[ImGuiCol_SliderGrab] = ImVec4(0.96862745f, 0.5882353f, 0.03529412f, 1.0f);
    rStyle.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.96862745f, 0.5882353f, 0.03529412f, 1.0f);
    rStyle.Colors[ImGuiCol_Button] = ImVec4(0.88235295f, 0.8666667f, 0.8509804f, 1.0f);
    rStyle.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.81960785f, 0.8117647f, 0.8039216f, 1.0f);
    rStyle.Colors[ImGuiCol_ButtonActive] = ImVec4(0.84705883f, 0.84705883f, 0.84705883f, 1.0f);
    rStyle.Colors[ImGuiCol_Header] = ImVec4(0.85882354f, 0.8352941f, 0.7921569f, 1.0f);
    rStyle.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.89411765f, 0.89411765f, 0.89411765f, 1.0f);
    rStyle.Colors[ImGuiCol_HeaderActive] = ImVec4(0.92156863f, 0.9137255f, 0.8980392f, 1.0f);
    rStyle.Colors[ImGuiCol_Separator] = ImVec4(0.87058824f, 0.8509804f, 0.80784315f, 1.0f);
    rStyle.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.84313726f, 0.8156863f, 0.7490196f, 1.0f);
    rStyle.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.84313726f, 0.8156863f, 0.7490196f, 1.0f);
    rStyle.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.85490197f, 0.85490197f, 0.85490197f, 1.0f);
    rStyle.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.96862745f, 0.050980393f, 0.15686275f, 1.0f);
    rStyle.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    rStyle.Colors[ImGuiCol_Tab] = ImVec4(0.92156863f, 0.9137255f, 0.8980392f, 1.0f);
    rStyle.Colors[ImGuiCol_TabHovered] = ImVec4(0.88235295f, 0.8666667f, 0.8509804f, 1.0f);
    rStyle.Colors[ImGuiCol_TabActive] = ImVec4(0.88235295f, 0.8666667f, 0.8509804f, 1.0f);
    rStyle.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.92156863f, 0.9137255f, 0.8980392f, 1.0f);
    rStyle.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.8745098f, 0.7254902f, 0.42745098f, 1.0f);
    rStyle.Colors[ImGuiCol_PlotLines] = ImVec4(0.47843137f, 0.4f, 0.29803923f, 1.0f);
    rStyle.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.9607843f, 0.019607844f, 0.11764706f, 1.0f);
    rStyle.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90588236f, 0.6627451f, 0.30980393f, 1.0f);
    rStyle.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.6392157f, 0.39607844f, 0.043137256f, 1.0f);
    rStyle.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.9529412f, 0.94509804f, 0.92941177f, 1.0f);
    rStyle.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.9529412f, 0.94509804f, 0.92941177f, 1.0f);
    rStyle.Colors[ImGuiCol_TableBorderLight] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    rStyle.Colors[ImGuiCol_TableRowBg] = ImVec4(0.88235295f, 0.8666667f, 0.8509804f, 1.0f);
    rStyle.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.9019608f, 0.89411765f, 0.8784314f, 1.0f);
    rStyle.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.0627451f, 0.0627451f, 0.0627451f, 1.0f);
    rStyle.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.5019608f, 0.4862745f, 0.0f, 1.0f);
    rStyle.Colors[ImGuiCol_NavHighlight] = ImVec4(0.73333335f, 0.70980394f, 0.0f, 1.0f);
    rStyle.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.5019608f, 0.4862745f, 0.0f, 1.0f);
    rStyle.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8039216f, 0.8235294f, 0.45490196f, 0.502f);
    rStyle.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.8039216f, 0.8235294f, 0.45490196f, 0.502f);
}
