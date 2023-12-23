#include "Runtime.h"

const PietToken Runtime::m_tDefaultToken = { PietToken::TokenType::Start };

void Runtime::StepExecution(PietToken& token, PietToken& value)
{
	int top = 0;
	int second = 0;
	int val = 0;

	switch (token.m_kind)
	{
	case(PietToken::TokenType::Push):
	{
		m_stack.Push(value.m_value);
		break;
	}
	case(PietToken::TokenType::Pop):
	{
		if (m_stack.GetSize() > 0)
		{
			m_stack.Pop();
		}
		break;
	}
	case(PietToken::TokenType::Add):
	{
		if (m_stack.GetSize() >= 2)
		{
			m_stack.Push(m_stack.Pop() + m_stack.Pop());
		}
		break;
	}
	case(PietToken::TokenType::Subtract):
	{
		if (m_stack.GetSize() >= 2)
		{
			top = m_stack.Pop();
			second = m_stack.Pop();
			m_stack.Push(second - top);
		}
		break;
	}
	case(PietToken::TokenType::Multiply):
	{
		if (m_stack.GetSize() >= 2)
		{
			m_stack.Push(m_stack.Pop() * m_stack.Pop());
		}
		break;
	}
	case(PietToken::TokenType::Divide):
	{
		if (m_stack.GetSize() >= 2)
		{
			top = m_stack.Pop();
			second = m_stack.Pop();
			m_stack.Push(second / top);
		}
		break;
	}
	case(PietToken::TokenType::Modulo):
	{
		if (m_stack.GetSize() >= 2)
		{
			top = m_stack.Pop();
			second = m_stack.Pop();
			m_stack.Push((top + (second % top)) % top);
		}
		break;
	}
	case(PietToken::TokenType::Not):
	{
		if (m_stack.GetSize() > 0)
		{
			val = m_stack.Pop() == 0 ? 1 : 0;
			m_stack.Push(val);
		}
		break;
	}
	case(PietToken::TokenType::Greater):
	{
		if (m_stack.GetSize() >= 2)
		{
			top = m_stack.Pop();
			second = m_stack.Pop();
			val = (second > top) ? 1 : 0;
			m_stack.Push(val);
		}
		break;
	}
	case(PietToken::TokenType::Pointer):
		break;
	case(PietToken::TokenType::Switch):
		break;
	case(PietToken::TokenType::Duplicate):
	{
		if (m_stack.GetSize() > 0)
		{
			val = m_stack.Pop();
			m_stack.Push(val);
			m_stack.Push(val);
		}
		break;
	}
	case(PietToken::TokenType::Roll):
	{
		if (m_stack.GetSize() >= 2)
		{
			top = m_stack.Pop();
			second = m_stack.Pop();
			m_stack.Roll(second, top);
		}
		break;
	}
	case(PietToken::TokenType::Input_Char):
		break;
	case(PietToken::TokenType::Input_Val):
		break;
	case(PietToken::TokenType::Output_Char):
	{
		if (m_stack.GetSize() > 0)
		{
			m_rOutputStr += (char)m_stack.Pop();
		}
		break;
	}
	case(PietToken::TokenType::Output_Val):
	{
		if (m_stack.GetSize() > 0)
		{
			m_rOutputStr += std::to_string(m_stack.Pop());
		}
		break;
	}
	case (PietToken::TokenType::End):
	{
		m_bFinished = true;
		break;
	}
	default:
		break;
	}
}

void Runtime::StepExecution()
{

	PietToken token = m_textTokeniser.Pop();
	PietToken value = m_tDefaultToken;
	if (token.m_kind == PietToken::TokenType::Push)
	{
		value = m_textTokeniser.Pop();
		if (value.m_kind != PietToken::TokenType::Value)
		{
			return;
		}
	}
	StepExecution(token, value);

	return;
}

int Runtime::Run()
{
	ResetStream();

	PietToken token = m_tDefaultToken;
	PietToken value = m_tDefaultToken;

	while (token.m_kind != PietToken::TokenType::End)
	{
		token = m_textTokeniser.Pop();
		if (token.m_kind == PietToken::TokenType::Push)
		{
			value = m_textTokeniser.Pop();
			if (value.m_kind != PietToken::TokenType::Value)
			{
				return -1;
			}
		}

		StepExecution(token, value);
	}

	return 0;
}