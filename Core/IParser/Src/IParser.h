#pragma once

#include <IAbstractSyntaxTree.h>
#include <ITokeniser.h>
#include <string>
#include <vector>

template<typename TokenClass>
class IParser
{
public:
	IParser(ITokeniser<TokenClass>* pTokeniser) : m_pTokeniser(pTokeniser) {}

	virtual IRegion Parse() = 0;

protected:
	bool Check(const TokenClass::TokenType::Enum expected)
	{
		return m_pTokeniser->Peek().m_type == expected;
	}

	bool Check(const std::vector<typename TokenClass::TokenType::Enum>& expected)
	{
		const std::vector<TokenClass> nextN = m_pTokeniser->Peek(expected.size());
		for (size_t tokenIdx = 0; tokenIdx < expected.size(); tokenIdx++)
		{
			if (nextN[tokenIdx].m_type != expected[tokenIdx])
			{
				return false;
			}
		}
		return true;
	}

	TokenClass Match(const TokenClass::TokenType::Enum expected)
	{
		if (Check(expected))
		{
			return m_pTokeniser->Pop();
		}
		std::string err = "Expected ";
		err = err + TokenClass::TokenType::ToString(expected) + " token not found";
		return Error(err.c_str());
	}

	TokenClass Error(const char* errorStr)
	{
		return TokenClass(TokenClass::TokenType::Unrecognised_Token);
	}

	ITokeniser<TokenClass>* m_pTokeniser{ nullptr };
};