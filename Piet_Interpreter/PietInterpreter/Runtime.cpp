#include "Runtime.h"

void Runtime::step_execution(PietToken& token, PietToken& value) {
	int top = 0;
	int second = 0;
	int val = 0;
	switch (token.kind)
	{
	case(PietToken::Kind::Push):
		stack.push(value.value);
		break;
	case(PietToken::Kind::Pop):
		if (stack.get_size() > 0) {
			stack.pop();
		}
		break;
	case(PietToken::Kind::Add):
		if (stack.get_size() >= 2) {
			stack.push(stack.pop() + stack.pop());
		}
		break;
	case(PietToken::Kind::Subtract):
		if (stack.get_size() >= 2) {
			top = stack.pop();
			second = stack.pop();
			stack.push(second - top);
		}
		break;
	case(PietToken::Kind::Multiply):
		if (stack.get_size() >= 2) {
			stack.push(stack.pop() * stack.pop());
		}
		break;
	case(PietToken::Kind::Divide):
		if (stack.get_size() >= 2) {
			top = stack.pop();
			second = stack.pop();
			stack.push(second / top);
		}
		break;
	case(PietToken::Kind::Modulo):
		if (stack.get_size() >= 2) {
			top = stack.pop();
			second = stack.pop();
			stack.push((top + (second % top)) % top);
		}
		break;
	case(PietToken::Kind::Not):
		if (stack.get_size() > 0) {
			val = stack.pop() == 0 ? 1 : 0;
			stack.push(val);
		}
		break;
	case(PietToken::Kind::Greater):
		if (stack.get_size() >= 2) {
			top = stack.pop();
			second = stack.pop();
			val = (second > top) ? 1 : 0;
			stack.push(val);
		}
		break;
	case(PietToken::Kind::Pointer):
		break;
	case(PietToken::Kind::Switch):
		break;
	case(PietToken::Kind::Duplicate):
		if (stack.get_size() > 0) {
			val = stack.pop();
			stack.push(val);
			stack.push(val);
		}
		break;
	case(PietToken::Kind::Roll):
		if (stack.get_size() >= 2) {
			top = stack.pop();
			second = stack.pop();
			stack.roll(second, top);
		}
		break;
	case(PietToken::Kind::Input_Char):
		break;
	case(PietToken::Kind::Input_Val):
		break;
	case(PietToken::Kind::Output_Char):
		if (stack.get_size() > 0) {
			output += (char)stack.pop();
		}
		break;
	case(PietToken::Kind::Output_Val):
		if (stack.get_size() > 0) {
			output += std::to_string(stack.pop());
		}
		break;
	case (PietToken::Kind::End):
		finished = true;
	default:
		break;
	}
}

void Runtime::step_execution() {
	PietToken token = startToken;
	PietToken value = startToken;
	if (token.kind != PietToken::Kind::End) {
		token = tk.pop();
		if (token.kind == PietToken::Kind::Push) {
			value = tk.pop();
			if (value.kind != PietToken::Kind::Value) {
				return;
			}
		}
		step_execution(token, value);
	}
	return;
}

int Runtime::run() {
	reset_stream();
	PietToken token = startToken;
	PietToken value = startToken;
	while (token.kind != PietToken::Kind::End) {
		token = tk.pop();
		if (token.kind == PietToken::Kind::Push) {
			value = tk.pop();
			if (value.kind != PietToken::Kind::Value) {
				return -1;
			}
		}
		step_execution(token, value);
	}
	return 0;
}