#include "PietTextTokeniser.h"
#include "PietStack.h"
#include <string>

class Runtime {

	enum class Direction {
		Up, Right, Down, Left
	};

	struct Location {
		int x;
		int y;
	};

	PietTextTokeniser tk;
	Direction dp;
	Direction cc;
	Location current_pixel { 0, 0 };

	bool finished = false;

	PietToken startToken{ PietToken::Kind::Start };

	PietStack stack;
	std::string code_str = "";
	std::istrstream code = std::istrstream("");

	std::string& output;

	void step_execution(PietToken& token, PietToken& value);

public:

	bool Is_Finished() { return finished; }

	Runtime(std::string& outputString) : output(outputString) {}

	void step_execution();

	void set_stream(std::string str) {
		code_str = str;
		code = std::istrstream(code_str.c_str());
		tk.set_stream(code);
		output = "";
		finished = false;
	}

	void reset_stream() {
		code = std::istrstream(code_str.c_str());
		tk.set_stream(code);
		output = "";
		finished = false;
	}

	int run();

	const std::deque<int>& getStack() { return stack.getStack(); }
};