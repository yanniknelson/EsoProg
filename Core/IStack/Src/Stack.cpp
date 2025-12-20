#include "Stack.h"
#include <vector>
#include <imgui.h>
#include <string>

void Stack::Clear()
{
	m_stack.clear();
}

int Stack::GetSize() const
{
	return (int)m_stack.size();
}

bool Stack::Empty() const
{
	return m_stack.empty();
}

void Stack::Push(int val)
{
	m_stack.push_front(val);
}

int Stack::Pop()
{
	int ret = m_stack.front();
	m_stack.pop_front();
	return ret;
}

void Stack::Roll(int depth, int rotations)
{

	int size = GetSize();
	depth = (size < depth) ? size : depth;
	rotations %= depth;

	if (rotations == 0 || depth <= 1)
	{
		return;
	}

	std::vector<int> values;
	values.reserve(depth);

	for (int i = 0; i < depth; i++)
	{
		values.push_back(m_stack.front());
		m_stack.pop_front();
	}

	for (int i = 0; i < depth; i++)
	{
		//int indx = (depth - 1 - rotations - i);
		int indx = (rotations - 1 - i);
		indx = (depth + (indx % depth)) % depth;
		m_stack.push_front(values[indx]);
	}
}

const std::deque<int>& Stack::GetStack() const
{
	return m_stack;
}

void Stack::DisplayStack() const
{
	if (ImGui::Begin("Stack"))
	{
		ImGui::Text("The rStack is displayed with the deepest value at the top");
		if (ImGui::BeginListBox("##Stack", ImGui::GetContentRegionAvail()))
		{
			const bool invertStack = false;
			const size_t stackSize = m_stack.size();
			for (size_t i = 1; i <= stackSize; i++)
			{
				std::string lbl = std::to_string(m_stack[invertStack ? stackSize - i : i - 1]) + "##" + std::to_string(i);
				ImGui::Selectable(lbl.c_str(), false, ImGuiSelectableFlags_Disabled);
			}
			ImGui::EndListBox();
		}
	}
	ImGui::End();
}