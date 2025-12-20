#pragma once

#include <imgui.h>

#include <climits>
#include <iostream>
#include <map>
#include <string>


template<typename memType>
class IMemoryArray
{

    std::map<int, memType> m_array;
    int m_maxIndex{ INT_MIN };
    int m_minIndex{ INT_MAX };
    int m_mainFocus{ 0 };

  public:
    void Clear()
    {
        m_array.clear();
        m_maxIndex = INT_MIN;
        m_minIndex = INT_MAX;
    };

    bool Empty() const
    {
        return m_array.empty();
    };

    memType Get(int indx) const
    {
        // quick check before running a find operation
        if (indx > m_maxIndex || indx < m_minIndex)
        {
            return static_cast<memType>(0);
        }

        if (typename std::map<int, memType>::const_iterator it = m_array.find(indx);
            it != m_array.end())
        {
            return it->second;
        }

        return static_cast<memType>(0);
    }

    void Set(int indx, memType val)
    {
        m_maxIndex = m_maxIndex > indx ? m_maxIndex : indx;
        m_minIndex = m_minIndex < indx ? m_minIndex : indx;
        m_array[indx] = val;
    }

    void Increment(int indx)
    {
        if (typename std::map<int, memType>::iterator it = m_array.find(indx);
            it != m_array.end())
        {
            ++(it->second);
        }
        else
        {
            m_array[indx] = static_cast<memType>(1);
        }
        m_maxIndex = m_maxIndex > indx ? m_maxIndex : indx;
        m_minIndex = m_minIndex < indx ? m_minIndex : indx;
    }

    void Decrement(int indx)
    {
        if (typename std::map<int, memType>::iterator it = m_array.find(indx);
            it != m_array.end())
        {
            --(it->second);
        }
        else
        {
            m_array[indx] = static_cast<memType>(-1);
        }
        m_maxIndex = m_maxIndex > indx ? m_maxIndex : indx;
        m_minIndex = m_minIndex < indx ? m_minIndex : indx;
    }

    friend std::ostream& operator<<(std::ostream& os, const IMemoryArray<memType>& arr)
    {
        for (typename std::map<int, memType>::const_iterator it : arr.m_array)
        {
            os << it->first << "," << it->second << std::endl;
        }
        return os;
    }

#define MemoryCellSize 50.f
#define Separation 10.f
#define MemoryCellHalfSize MemoryCellSize / 2.f

    void DisplayArray(const int index) const
    {
        if (ImGui::Begin("Memory Array"))
        {
            const ImVec2 zeroCharSize = ImGui::CalcTextSize("0", 0, true, MemoryCellSize - Separation);
            constexpr float tan60 = 1.7320508f; // approximation of tan(60) or sqrt(3)
            const float triangleHalfWidth = ImGui::GetFontSize() / 2.f;
            const float triangleHieght = tan60 * triangleHalfWidth; // (hight = tan(60) * width/2)

            const float labelSize = triangleHieght + zeroCharSize.y + (2 * Separation);
            const float contentHieght = MemoryCellSize + labelSize;

            ImGuiStyle& style = ImGui::GetStyle();
            const ImVec2 windowPos = ImGui::GetCursorScreenPos();
            const ImVec2 windowSize = ImGui::GetContentRegionAvail();
            const ImVec2 windowCenter(windowPos.x + (windowSize.x / 2), windowPos.y + (windowSize.y / 2));
            const float contentTopYPos = windowCenter.y - (contentHieght / 2.f);
            const float cellYPos = contentTopYPos + labelSize;
            const size_t halfNumCells = static_cast<size_t>(((windowSize.x / MemoryCellSize) - 1.5f) / 2.f);
            const size_t numCells = (halfNumCells << 1) + 1;
            const float cellStartXPos = windowCenter.x - (numCells / 2.f * MemoryCellSize);
            ImDrawList* pDrawList = ImGui::GetWindowDrawList();
            for (size_t cellNum = 0; cellNum < numCells; cellNum++)
            {
                const ImVec2 rectMin(cellStartXPos + cellNum * MemoryCellSize, cellYPos);
                const ImVec2 rectMax(rectMin.x + MemoryCellSize, rectMin.y + MemoryCellSize);
                const int cellIndex = index + static_cast<int>(cellNum - halfNumCells);
                float extraHeight = 0.f;
                if (cellIndex == index)
                {
                    const ImVec2 traingleTopLeft(rectMin.x + MemoryCellHalfSize - triangleHalfWidth, rectMin.y - triangleHieght - Separation);
                    const ImVec2 traingleTopRight(traingleTopLeft.x + triangleHalfWidth * 2, traingleTopLeft.y);
                    const ImVec2 trainglePoint(traingleTopLeft.x + triangleHalfWidth, rectMin.y - Separation);
                    pDrawList->AddTriangleFilled(traingleTopLeft, trainglePoint, traingleTopRight, ImColor(style.Colors[ImGuiCol_Text]));
                    extraHeight = triangleHieght + Separation;
                }
                const ImVec2 cellIndexTextSize = ImGui::CalcTextSize(std::to_string(cellIndex).c_str(), 0, true, MemoryCellSize - Separation);
                const ImVec2 cellIndexTextPos(rectMin.x + MemoryCellHalfSize - cellIndexTextSize.x / 2.f, rectMin.y - cellIndexTextSize.y - Separation - extraHeight);
                pDrawList->AddText(cellIndexTextPos, ImColor(style.Colors[ImGuiCol_Text]), std::to_string(cellIndex).c_str());
                pDrawList->AddRectFilled(rectMin, rectMax, ImColor(style.Colors[ImGuiCol_FrameBg]), 4.0f);
                pDrawList->AddRect(rectMin, rectMax, ImColor(style.Colors[ImGuiCol_Border]), 4.0f);
                const memType val = Get(cellIndex);
                ImVec2 valTextSize = zeroCharSize;
                const std::string valStr = std::to_string(val);
                if (val != 0)
                {
                    valTextSize = ImGui::CalcTextSize(valStr.c_str(), 0, true, MemoryCellSize - Separation);
                }
                const ImVec2 valTextPos(rectMin.x + MemoryCellHalfSize - valTextSize.x / 2.f, rectMin.y + MemoryCellHalfSize - valTextSize.y / 2.f);
                pDrawList->AddText(valTextPos, ImColor(style.Colors[ImGuiCol_Text]), valStr.c_str());
            }
        }
        ImGui::End();
    }
};
