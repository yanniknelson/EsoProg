#include "PietToken.h"

class PietTokeniser
{
protected:
	PietToken m_tLastPopped{ PietToken::TokenType::End };
public:

	/// <summary>
	/// Get the next Token
	/// </summary>
	/// <returns> The next PietToken to be executed </returns>
	virtual const PietToken& Pop() = 0;

	/// <summary>
	/// Get the last Token popped
	/// </summary>
	/// <returns> The last token returned by a call of Pop </returns>
	virtual const PietToken& LastPopped() const { return m_tLastPopped; };
};