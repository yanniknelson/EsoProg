#include "Runtime.h"

const PietToken Runtime::m_tDefaultToken = { PietToken::TokenType::Start };

void Runtime::StepExecution(PietToken& token)
{
	int top = 0;
	int second = 0;
	int val = 0;

	std::cout << token << std::endl;

	switch (token.m_type)
	{
	case(PietToken::TokenType::Push):
	{
		m_stack.Push(token.m_value);
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
	{
		if (!m_stack.Empty())
		{
			top = m_stack.Pop();
			m_imageTokeniser.RotateDirectionPointer(top);
		}
		break;
	}
	case(PietToken::TokenType::Switch):
	{
		if (!m_stack.Empty())
		{
			top = m_stack.Pop();
			if (top % 2 != 0)
			{
				m_imageTokeniser.ToggleCodelChooser();
			}
		}
		break;
	}
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
		m_waitingForCharInput = true;
		break;
	case(PietToken::TokenType::Input_Val):
		m_waitingForValInput = true;
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
	case(PietToken::TokenType::NOP):
	{
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

void Runtime::InputChar(int val)
{
	m_stack.Push(val);
	m_waitingForCharInput = false;
}

void Runtime::InputVal(int val)
{
	m_stack.Push(val);
	m_waitingForValInput = false;
}

bool Runtime::IsWaitingForValInput() const
{
	return m_waitingForValInput;
}

bool Runtime::IsWaitingForCharInput() const
{
	return m_waitingForCharInput;
}

void Runtime::StepExecution(SourceType sourceType)
{
	if (m_waitingForCharInput || m_waitingForValInput)
	{
		return;
	}

	PietToken token = m_tDefaultToken;
	PietToken value = m_tDefaultToken;

	switch (sourceType)
	{
	case(SourceType::Text):
	{
		token = m_textTokeniser.Pop();
		value = m_tDefaultToken;
		if (token.m_type == PietToken::TokenType::Push)
		{
			value = m_textTokeniser.Pop();
			if (value.m_type != PietToken::TokenType::Value)
			{
				return;
			}
			token.m_value = value.m_value;
		}
	}
	case(SourceType::Image):
	{
		token = m_imageTokeniser.Pop();
		// To make stepping a little faster could make optional as may confuse flow?
		while (token.m_type == PietToken::TokenType::NOP)
		{
			token = m_imageTokeniser.Pop();
		}
		break;
	}
	}

	StepExecution(token);

	return;
}

void Runtime::SetCodelSize(const int size)
{
	m_imageTokeniser.SetCodelSize(size);
}

int Runtime::RunFromStart(SourceType sourceType)
{
	ResetTokenisers();

	m_currentSourceType = sourceType;

	return Run();
}

int Runtime::Run()
{
	

	PietToken token = m_tDefaultToken;
	PietToken value = m_tDefaultToken;

	while (!m_bFinished)
	{
		switch (m_currentSourceType)
		{
		case(SourceType::Text):
		{
			token = m_textTokeniser.Pop();
			if (token.m_type == PietToken::TokenType::Push)
			{
				value = m_textTokeniser.Pop();
				if (value.m_type != PietToken::TokenType::Value)
				{
					return -1;
				}
			}
			token.m_value = value.m_value;
			break;
		}
		case(SourceType::Image):
		{
			token = m_imageTokeniser.Pop();
			break;
		}
		}

		StepExecution(token);

		if (m_waitingForCharInput || m_waitingForValInput)
		{
			return 0;
		}
	}

	return 0;
}