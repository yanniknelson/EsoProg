#include "Tokeniser.h"

Tokeniser::Token& Tokeniser::pop() {
	char ch = ' ';
	std::string word;
	while (isspace(ch)) {
		stream->get(ch);
	}
	switch (ch)
	{
	case('0'):
	case('1'):
	case('2'):
	case('3'):
	case('4'):
	case('5'):
	case('6'):
	case('7'):
	case('8'):
	case('9'):
		stream->putback(ch);
		lp.kind = Kind::Value;
		*stream>>lp.value;
		break;
	default:
		if (isalpha(ch)) {
			stream->putback(ch);
			*stream >> word;
			if (word == "PUSH") {
				lp.kind = Kind::Push;
				lp.value = NAN;
			} else if (word == "POP") {
				lp.kind = Kind::Pop;
				lp.value = NAN;
			} else if (word == "Add") {
				lp.kind = Kind::Add;
				lp.value = NAN;
			} else if (word == "SUB") {
				lp.kind = Kind::Subtract;
				lp.value = NAN;
			} else if (word == "MUL") {
				lp.kind = Kind::Multiply;
				lp.value = NAN;
			} else if (word == "DIV") {
				lp.kind = Kind::Divide;
				lp.value = NAN;
			} else if (word == "MOD") {
				lp.kind = Kind::Modulo;
				lp.value = NAN;
			} else if (word == "NOT") {
				lp.kind = Kind::Not;
				lp.value = NAN;
			} else if (word == "GR") {
				lp.kind = Kind::Greater;
				lp.value = NAN;
			} else if (word == "PNTR") {
				lp.kind = Kind::Pointer;
				lp.value = NAN;
			} else if (word == "SWTCH") {
				lp.kind = Kind::Switch;
				lp.value = NAN;
			} else if (word == "DUP") {
				lp.kind = Kind::Duplicate;
				lp.value = NAN;
			} else if (word == "ROLL") {
				lp.kind = Kind::Roll;
				lp.value = NAN;
			} else if (word == "OUT") {
				word = "";
				*stream >> word;
				if (word == "CHAR") {
					lp.kind = Kind::Output_Char;
					lp.value = NAN;
				} else if (word == "INT") {
					lp.kind = Kind::Output_Val;
					lp.value = NAN;
				}
			} else if (word == "IN") {
				word = "";
				*stream >> word;
				if (word == "CHAR") {
					lp.kind = Kind::Input_Char;
					lp.value = NAN;
				} else if (word == "INT") {
					lp.kind = Kind::Input_Val;
					lp.value = NAN;
				}
			}
			else {
				lp.kind = Kind::End;
				lp.value = NAN;
			}
		}
	}
	return lp;
}

Tokeniser::Token& Tokeniser::last_popped() {
	return lp;
}