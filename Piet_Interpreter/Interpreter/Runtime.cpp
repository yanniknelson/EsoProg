#include "Runtime.h"

void Runtime::step_execution(Tokeniser::Token& token, Tokeniser::Token& value) {
	int top = 0;
	int second = 0;
	int val = 0;
	switch (token.kind)
	{
	case(Tokeniser::Kind::Push):
		stack.push(value.value);
		break;
	case(Tokeniser::Kind::Pop):
		if (stack.get_size() > 0) {
			stack.pop();
		}
		break;
	case(Tokeniser::Kind::Add):
		if (stack.get_size() >= 2) {
			stack.push(stack.pop() + stack.pop());
		}
		break;
	case(Tokeniser::Kind::Subtract):
		if (stack.get_size() >= 2) {
			top = stack.pop();
			second = stack.pop();
			stack.push(second - top);
		}
		break;
	case(Tokeniser::Kind::Multiply):
		if (stack.get_size() >= 2) {
			stack.push(stack.pop() * stack.pop());
		}
		break;
	case(Tokeniser::Kind::Divide):
		if (stack.get_size() >= 2) {
			top = stack.pop();
			second = stack.pop();
			stack.push(second / top);
		}
		break;
	case(Tokeniser::Kind::Modulo):
		if (stack.get_size() >= 2) {
			top = stack.pop();
			second = stack.pop();
			stack.push((top + (second % top)) % top);
		}
		break;
	case(Tokeniser::Kind::Not):
		if (stack.get_size() > 0) {
			val = stack.pop() == 0 ? 1 : 0;
			stack.push(val);
		}
		break;
	case(Tokeniser::Kind::Greater):
		if (stack.get_size() >= 2) {
			top = stack.pop();
			second = stack.pop();
			val = (second > top) ? 1 : 0;
			stack.push(val);
		}
		break;
	case(Tokeniser::Kind::Pointer):
		break;
	case(Tokeniser::Kind::Switch):
		break;
	case(Tokeniser::Kind::Duplicate):
		if (stack.get_size() > 0) {
			val = stack.pop();
			stack.push(val);
			stack.push(val);
		}
		break;
	case(Tokeniser::Kind::Roll):
		if (stack.get_size() >= 2) {
			top = stack.pop();
			second = stack.pop();
			stack.roll(second, top);
		}
		break;
	case(Tokeniser::Kind::Input_Char):
		break;
	case(Tokeniser::Kind::Input_Val):
		break;
	case(Tokeniser::Kind::Output_Char):
		if (stack.get_size() > 0) {
			std::cout << (char)stack.pop();
		}
		break;
	case(Tokeniser::Kind::Output_Val):
		if (stack.get_size() > 0) {
			std::cout << stack.pop();
		}
		break;
	default:
		break;
	}
}

int Runtime::run() {
	Tokeniser::Token token = startToken;
	Tokeniser::Token value = startToken;
	while (token.kind != Tokeniser::Kind::End) {
		token = tk.pop();
		if (token.kind == Tokeniser::Kind::Push) {
			value = tk.pop();
			if (value.kind != Tokeniser::Kind::Value) {
				return -1;
			}
		}
		step_execution(token, value);
	}
	return 0;
}