#include "PietTextTokeniser.h"

PietToken& PietTextTokeniser::pop() {
	if (!stream->rdbuf()->in_avail()) {
		lp.kind = PietToken::Kind::End;
		lp.value = NAN;
		return lp;
	}
	char ch = ' ';
	std::string word;
	while (isspace(ch)) {
		stream->get(ch);
		if (ch == '\n') { line_number++; }
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
		lp.kind = PietToken::Kind::Value;
		*stream>>lp.value;
		break;
	default:
		if (isalpha(ch)) {
			stream->putback(ch);
			*stream >> word;
			if (word == "PUSH") {
				lp.kind = PietToken::Kind::Push;
				lp.value = NAN;
			} else if (word == "POP") {
				lp.kind = PietToken::Kind::Pop;
				lp.value = NAN;
			} else if (word == "Add") {
				lp.kind = PietToken::Kind::Add;
				lp.value = NAN;
			} else if (word == "SUB") {
				lp.kind = PietToken::Kind::Subtract;
				lp.value = NAN;
			} else if (word == "MUL") {
				lp.kind = PietToken::Kind::Multiply;
				lp.value = NAN;
			} else if (word == "DIV") {
				lp.kind = PietToken::Kind::Divide;
				lp.value = NAN;
			} else if (word == "MOD") {
				lp.kind = PietToken::Kind::Modulo;
				lp.value = NAN;
			} else if (word == "NOT") {
				lp.kind = PietToken::Kind::Not;
				lp.value = NAN;
			} else if (word == "GR") {
				lp.kind = PietToken::Kind::Greater;
				lp.value = NAN;
			} else if (word == "PNTR") {
				lp.kind = PietToken::Kind::Pointer;
				lp.value = NAN;
			} else if (word == "SWTCH") {
				lp.kind = PietToken::Kind::Switch;
				lp.value = NAN;
			} else if (word == "DUP") {
				lp.kind = PietToken::Kind::Duplicate;
				lp.value = NAN;
			} else if (word == "ROLL") {
				lp.kind = PietToken::Kind::Roll;
				lp.value = NAN;
			} else if (word == "END") {
				lp.kind = PietToken::Kind::End;
				lp.value = NAN;
			} else if (word == "OUT") {
				word = "";
				*stream >> word;
				if (word == "CHAR") {
					lp.kind = PietToken::Kind::Output_Char;
					lp.value = NAN;
				} else if (word == "INT") {
					lp.kind = PietToken::Kind::Output_Val;
					lp.value = NAN;
				} else {
					lp.kind = PietToken::Kind::Unrecognised_Token;
					lp.value = NAN;
				}
			} else if (word == "IN") {
				word = "";
				*stream >> word;
				if (word == "CHAR") {
					lp.kind = PietToken::Kind::Input_Char;
					lp.value = NAN;
				} else if (word == "INT") {
					lp.kind = PietToken::Kind::Input_Val;
					lp.value = NAN;
				} else {
					lp.kind = PietToken::Kind::Unrecognised_Token;
					lp.value = NAN;
				}
			} else {
				lp.kind = PietToken::Kind::Unrecognised_Token;
				lp.value = NAN;
			}
		}
	}
	return lp;
}

PietToken& PietTextTokeniser::last_popped() {
	return lp;
}