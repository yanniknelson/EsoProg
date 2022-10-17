#include <strstream>
#include <iostream>

#include "PietToken.h"

class PietTextTokeniser {
public:

	PietToken& pop();
	PietToken& last_popped();

	PietTextTokeniser() {}

	void set_stream(std::istrstream& code) {
		stream = &code;
		line_number = 1;
	}

	int get_line_number() {
		return line_number;
	}

private:

	std::istrstream* stream;
	PietToken lp{ PietToken::Kind::End };
	int line_number = 1;

};