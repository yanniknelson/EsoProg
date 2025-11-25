#pragma once

#include <map>
#include <iostream>

#include <imgui.h>

template<typename memType>
class IMemoryArray {

	std::map<int, memType> m_array;
	int m_maxIndex{ 0 };
	int m_minIndex{ 0 };
	int m_mainFocus{ 0 };
public:

	void Clear() {
		m_array.clear();
		m_maxIndex = 0;
		m_minIndex = 0;
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
			(*it)++;
		}
		else
		{
			m_array[indx] = static_cast<memType>(1);
		}
		m_maxIndex = m_maxIndex > indx ? m_maxIndex : indx;
		m_minIndex = m_minIndex < indx ? m_minIndex : indx;
	}

	int Decrement(int indx)
	{
		if (typename std::map<int, memType>::iterator it = m_array.find(indx);
			it != m_array.end())
		{
			(*it)--;
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

	void DisplayArray() const
	{
		if (ImGui::Begin("Memory Array"))
		{
			const ImVec2 windowPos = ImGui::GetCursorScreenPos();
			const ImVec2 windowSize = ImGui::GetContentRegionAvail();
			const ImVec2 windowCenter(windowPos.x + (windowSize.x / 2), windowPos.y + (windowSize.y / 2));
			ImDrawList* pDrawList = ImGui::GetWindowDrawList();
			pDrawList->AddRectFilled(windowCenter, ImVec2(windowCenter.x + 20, windowCenter.y + 20), IM_COL32(75, 75, 75, 255), 4.0f);
			pDrawList->AddRect(windowCenter, ImVec2(windowCenter.x + 20, windowCenter.y + 20), IM_COL32(100, 100, 100, 255), 4.0f);
		}
		ImGui::End();
	}
};