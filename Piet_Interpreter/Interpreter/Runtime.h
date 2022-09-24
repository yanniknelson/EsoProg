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
	Location current_pixel { 0,0 };

	Tokeniser::Token startToken{ Tokeniser::Kind::Start };

	PietStack stack;

	void step_execution(Tokeniser::Token& token, Tokeniser::Token& value);

public:

	void set_stream(std::istrstream& input) {
		tk.set_stream(input);
	}

	int run();
};