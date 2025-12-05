#pragma once

#include <ITokeniser.h>

template<typename TokenClass>
class IAST
{
public:

	void Parse()
	{
	};

protected:
	ITokeniser<TokenClass> m_tokeniser;
};