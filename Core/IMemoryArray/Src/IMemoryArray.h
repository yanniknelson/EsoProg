#pragma once

#include <map>
#include <limits>
#include <iostream>
#include <string>

#include <imgui.h>

template<typename memType>
class IMemoryArray {

	std::map<int, memType> m_array;
	int m_maxIndex{ INT_MIN };
	int m_minIndex{ INT_MAX };
	int m_mainFocus{ 0 };
public:

	void Clear() {
		m_array.clear();
		m_maxIndex = INT_MIN;
		m_minIndex = INT_MAX;
	};

	bool Empty() const { return m_array.empty(); };

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
#define MemoryCellHalfSize MemoryCellSize/2.f

	void DisplayArray(const int index) const
	{
		if (ImGui::Begin("Memory Array"))
		{
			ImGuiStyle& style = ImGui::GetStyle();
			const ImVec2 windowPos = ImGui::GetCursorScreenPos();
			const ImVec2 windowSize = ImGui::GetContentRegionAvail();
			const ImVec2 windowCenter(windowPos.x + (windowSize.x / 2), windowPos.y + (windowSize.y / 2));
			const float cellYPos = windowCenter.y - MemoryCellHalfSize;
			const size_t halfNumCells = ((windowSize.x / MemoryCellSize) - 1.5)/2;
			const size_t numCells = (halfNumCells << 1) + 1;
			const float cellStartXPos = windowCenter.x - (numCells/ 2.f * MemoryCellSize);
			ImDrawList* pDrawList = ImGui::GetWindowDrawList();
			for (size_t cellNum = 0; cellNum < numCells; cellNum++)
			{
				const ImVec2 rectMin(cellStartXPos + cellNum * MemoryCellSize, cellYPos);
				const ImVec2 rectMax(rectMin.x + MemoryCellSize, rectMin.y + MemoryCellSize);
				const int cellIndex = index - halfNumCells + cellNum;
				const ImVec2 cellIndexTextSize = ImGui::CalcTextSize(std::to_string(cellIndex).c_str(), 0, true, MemoryCellSize - 10);
				const ImVec2 cellIndexTextPos(rectMin.x + MemoryCellHalfSize - cellIndexTextSize.x / 2.f, rectMin.y - cellIndexTextSize.y - 10.f);
				pDrawList->AddText(cellIndexTextPos, ImColor(style.Colors[ImGuiCol_Text]), std::to_string(cellIndex).c_str());
				pDrawList->AddRectFilled(rectMin, rectMax, ImColor(style.Colors[ImGuiCol_FrameBg]), 4.0f);
				pDrawList->AddRect(rectMin, rectMax, ImColor(style.Colors[ImGuiCol_Border]), 4.0f);
				const std::string val = std::to_string(Get(cellIndex));
				const ImVec2 valTextSize = ImGui::CalcTextSize(val.c_str(), 0, true, MemoryCellSize - 10);
				const ImVec2 valTextPos(rectMin.x + MemoryCellHalfSize - valTextSize.x / 2.f, rectMin.y + MemoryCellHalfSize - valTextSize.y / 2.f);
				pDrawList->AddText(valTextPos, ImColor(style.Colors[ImGuiCol_Text]), val.c_str());
			}
		}
		ImGui::End();
	}
};