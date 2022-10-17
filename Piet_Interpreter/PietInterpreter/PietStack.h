#include <deque>
#include <iostream>

class PietStack {

	std::deque<int> stack;

public:

	int get_size() {return (int) stack.size(); }
	void push(int val) { stack.push_front(val); }
	int pop();

	//do not call roll(pop(), pop()); c++ 17 doesn't guarantee the pops will be called left to right
	void roll(int depth, int rotations);

	friend std::ostream& operator<<(std::ostream& os, const PietStack& pt) {
		for (auto it = pt.stack.begin(); it != pt.stack.end(); it++) {
			os << *it << " ";
		}
		return os;
	}

	const std::deque<int>& getStack() { return stack; }

};