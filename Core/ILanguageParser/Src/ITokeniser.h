#pragma once

template<typename TokenClass>
class ITokeniser
{
public:
	/// <summary>
	/// Get the next Token
	/// </summary>
	/// <returns> The next Token to be executed </returns>
	virtual const TokenClass& Pop() = 0;

	/// <summary>
	/// Get the last Token popped
	/// </summary>
	/// <returns> The last token returned by a call of Pop </returns>
	virtual const TokenClass& LastPopped() const { return m_tLastPopped; };

protected:
	TokenClass m_tLastPopped{ TokenClass::TokenType::End };
};