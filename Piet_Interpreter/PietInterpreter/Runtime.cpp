#include "Runtime.h"

void Runtime::step_execution(Token& token, Token& value) {
	int top = 0;
	int second = 0;
	int val = 0;
	switch (token.kind)
	{
	case(Token::Kind::Push):
		stack.push(value.value);
		break;
	case(Token::Kind::Pop):
		if (stack.get_size() > 0) {
			stack.pop();
		}
		break;
	case(Token::Kind::Add):
		if (stack.get_size() >= 2) {
			stack.push(stack.pop() + stack.pop());
		}
		break;
	case(Token::Kind::Subtract):
		if (stack.get_size() >= 2) {
			top = stack.pop();
			second = stack.pop();
			stack.push(second - top);
		}
		break;
	case(Token::Kind::Multiply):
		if (stack.get_size() >= 2) {
			stack.push(stack.pop() * stack.pop());
		}
		break;
	case(Token::Kind::Divide):
		if (stack.get_size() >= 2) {
			top = stack.pop();
			second = stack.pop();
			stack.push(second / top);
		}
		break;
	case(Token::Kind::Modulo):
		if (stack.get_size() >= 2) {
			top = stack.pop();
			second = stack.pop();
			stack.push((top + (second % top)) % top);
		}
		break;
	case(Token::Kind::Not):
		if (stack.get_size() > 0) {
			val = stack.pop() == 0 ? 1 : 0;
			stack.push(val);
		}
		break;
	case(Token::Kind::Greater):
		if (stack.get_size() >= 2) {
			top = stack.pop();
			second = stack.pop();
			val = (second > top) ? 1 : 0;
			stack.push(val);
		}
		break;
	case(Token::Kind::Pointer):
		break;
	case(Token::Kind::Switch):
		break;
	case(Token::Kind::Duplicate):
		if (stack.get_size() > 0) {
			val = stack.pop();
			stack.push(val);
			stack.push(val);
		}
		break;
	case(Token::Kind::Roll):
		if (stack.get_size() >= 2) {
			top = stack.pop();
			second = stack.pop();
			stack.roll(second, top);
		}
		break;
	case(Token::Kind::Input_Char):
		break;
	case(Token::Kind::Input_Val):
		break;
	case(Token::Kind::Output_Char):
		if (stack.get_size() > 0) {
			output += (char)stack.pop();
		}
		break;
	case(Token::Kind::Output_Val):
		if (stack.get_size() > 0) {
			output += std::to_string(stack.pop());
		}
		break;
	case (Token::Kind::End):
		finished = true;
	default:
		break;
	}
}

void Runtime::step_execution() {
	Token token = startToken;
	Token value = startToken;
	if (token.kind != Token::Kind::End) {
		token = tk.pop();
		if (token.kind == Token::Kind::Push) {
			value = tk.pop();
			if (value.kind != Token::Kind::Value) {
				return;
			}
		}
		step_execution(token, value);
	}
	return;
}

int Runtime::run() {
	reset_stream();
	Token token = startToken;
	Token value = startToken;
	while (token.kind != Token::Kind::End) {
		token = tk.pop();
		if (token.kind == Token::Kind::Push) {
			value = tk.pop();
			if (value.kind != Token::Kind::Value) {
				return -1;
			}
		}
		step_execution(token, value);
	}
	return 0;
}