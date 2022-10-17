#include "Tokeniser.h"

Token& Tokeniser::pop() {
	if (!stream->rdbuf()->in_avail()) {
		lp.kind = Token::Kind::End;
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
		lp.kind = Token::Kind::Value;
		*stream>>lp.value;
		break;
	default:
		if (isalpha(ch)) {
			stream->putback(ch);
			*stream >> word;
			if (word == "PUSH") {
				lp.kind = Token::Kind::Push;
				lp.value = NAN;
			} else if (word == "POP") {
				lp.kind = Token::Kind::Pop;
				lp.value = NAN;
			} else if (word == "Add") {
				lp.kind = Token::Kind::Add;
				lp.value = NAN;
			} else if (word == "SUB") {
				lp.kind = Token::Kind::Subtract;
				lp.value = NAN;
			} else if (word == "MUL") {
				lp.kind = Token::Kind::Multiply;
				lp.value = NAN;
			} else if (word == "DIV") {
				lp.kind = Token::Kind::Divide;
				lp.value = NAN;
			} else if (word == "MOD") {
				lp.kind = Token::Kind::Modulo;
				lp.value = NAN;
			} else if (word == "NOT") {
				lp.kind = Token::Kind::Not;
				lp.value = NAN;
			} else if (word == "GR") {
				lp.kind = Token::Kind::Greater;
				lp.value = NAN;
			} else if (word == "PNTR") {
				lp.kind = Token::Kind::Pointer;
				lp.value = NAN;
			} else if (word == "SWTCH") {
				lp.kind = Token::Kind::Switch;
				lp.value = NAN;
			} else if (word == "DUP") {
				lp.kind = Token::Kind::Duplicate;
				lp.value = NAN;
			} else if (word == "ROLL") {
				lp.kind = Token::Kind::Roll;
				lp.value = NAN;
			} else if (word == "END") {
				lp.kind = Token::Kind::End;
				lp.value = NAN;
			} else if (word == "OUT") {
				word = "";
				*stream >> word;
				if (word == "CHAR") {
					lp.kind = Token::Kind::Output_Char;
					lp.value = NAN;
				} else if (word == "INT") {
					lp.kind = Token::Kind::Output_Val;
					lp.value = NAN;
				} else {
					lp.kind = Token::Kind::Unrecognised_Token;
					lp.value = NAN;
				}
			} else if (word == "IN") {
				word = "";
				*stream >> word;
				if (word == "CHAR") {
					lp.kind = Token::Kind::Input_Char;
					lp.value = NAN;
				} else if (word == "INT") {
					lp.kind = Token::Kind::Input_Val;
					lp.value = NAN;
				} else {
					lp.kind = Token::Kind::Unrecognised_Token;
					lp.value = NAN;
				}
			} else {
				lp.kind = Token::Kind::Unrecognised_Token;
				lp.value = NAN;
			}
		}
	}
	return lp;
}

Token& Tokeniser::last_popped() {
	return lp;
}