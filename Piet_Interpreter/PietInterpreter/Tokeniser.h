#include <strstream>
#include <iostream>

#include "Token.h"

class Tokeniser {
public:

	Token& pop();
	Token& last_popped();

	Tokeniser() {}

	void set_stream(std::istrstream& code) {
		stream = &code;
		line_number = 1;
	}

	int get_line_number() {
		return line_number;
	}

private:

	std::istrstream* stream;
	Token lp{ Token::Kind::End };
	int line_number = 1;

};