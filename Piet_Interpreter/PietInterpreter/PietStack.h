#include <deque>
#include <iostream>

class PietStack {

	std::deque<int> m_stack;

public:

	/// <summary>
	/// Remove all elements from the stack
	/// </summary>
	void Clear();

	/// <summary>
	/// Get the size of the stack
	/// </summary>
	/// <returns> The number of elements in the stack </returns>
	int GetSize() const;

	/// <summary>
	/// Add a value onto the top of the stack
	/// </summary>
	/// <param name="val -"> The value that will be placed on the top of the stack </param>
	void Push(int val);

	/// <summary>
	/// Get and remove the value on the top of the stack
	/// </summary>
	/// <returns> The value from the top of the stack </returns>
	int Pop();

	/// <summary>
	/// Rotate a section of the stack starting from the front
	/// e.g. For a stak [1, 2, 3, 4] calling Roll(3,1) will result in the stack becoming [3, 1, 2, 4]
	/// WARNDING: do not call Roll(Pop(), Pop()); c++ 17 doesn't guarantee the pops will be called left to right
	/// </summary>
	/// <param name="depth -"> The number of elements, from the top, that will be rolled </param>
	/// <param name="rotations -"> The number of steps each element will move </param>
	void Roll(int depth, int rotations);

	/// <summary>
	/// Format and output the context of the stack for and output stream
	/// </summary>
	/// <param name="os - "> The output stream </param>
	/// <param name="pt - "> The piet stack </param>
	/// <returns></returns>
	friend std::ostream& operator<<(std::ostream& os, const PietStack& pt) {
		for (auto it = pt.m_stack.begin(); it != pt.m_stack.end(); it++) {
			os << *it << " ";
		}
		return os;
	}

	/// <summary>
	/// Get a const reference to the stack data (not manually editable)
	/// </summary>
	/// <returns> A const reference to the stack data </returns>
	const std::deque<int>& GetStack() const;
};