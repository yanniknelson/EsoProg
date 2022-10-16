#include "Tokeniser.h"
#include "PietStack.h"

class Runtime {

	enum class Direction {
		Up, Right, Down, Left
	};

	struct Location {
		int x;
		int y;
	};

	Tokeniser tk;
	Direction dp;
	Direction cc;
	Location current_pixel { 0, 0 };

	Tokeniser::Token startToken{ Tokeniser::Kind::Start };

	PietStack stack;
	std::string code_str = "";
	std::istrstream code = std::istrstream("");;

	void step_execution(Tokeniser::Token& token, Tokeniser::Token& value);

public:

	void step_execution();

	void set_stream(std::string str) {
		code_str = str;
		code = std::istrstream(code_str.c_str());
		tk.set_stream(code);
	}

	int run();

	const std::deque<int>& getStack() { return stack.getStack(); }
};