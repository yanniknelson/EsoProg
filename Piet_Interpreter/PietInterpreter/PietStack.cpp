#include "PietStack.h"
#include <vector>

int PietStack::pop() { 
	int ret = stack.front(); 
	stack.pop_front(); 
	return ret;
}

void PietStack::roll(int depth, int rotations) {
	int size = get_size();
	depth = (size < depth) ? size : depth;
	rotations %= depth;
	if (rotations == 0 || depth <= 1) { return; }
	std::vector<int> values;
	values.reserve(depth);
	for (int i = 0; i < depth; i++) {
		values.push_back(stack.front());
		stack.pop_front();
	}
	for (int i = 0; i < depth; i++) {
		int indx = (depth - 1 - rotations - i);
		indx = (depth + (indx % depth)) % depth;
		stack.push_front(values[indx]);
	}
}
