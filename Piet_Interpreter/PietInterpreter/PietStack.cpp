#include "PietStack.h"
#include <vector>

void PietStack::Clear()
{
	m_stack.clear();
}

int PietStack::GetSize() const
{
	return (int)m_stack.size();
}

bool PietStack::Empty() const
{
	return m_stack.empty();
}

void PietStack::Push(int val)
{
	m_stack.push_front(val);
}

int PietStack::Pop()
{
	int ret = m_stack.front();
	m_stack.pop_front();
	return ret;
}

void PietStack::Roll(int depth, int rotations)
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
		int indx = (depth - 1 - rotations - i);
		indx = (depth + (indx % depth)) % depth;
		m_stack.push_front(values[indx]);
	}
}

const std::deque<int>& PietStack::GetStack() const
{
	return m_stack;
}
