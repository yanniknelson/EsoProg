#pragma once

template<typename TokenClass>
class ITokeniser
{
  public:
    /// <summary>
    /// Get the next token
    /// </summary>
    /// <returns> The next token </returns>
    const TokenClass& Pop()
    {
        return Pop_Internal();
        ;
    }

    /// <summary>
    /// Get the last Token popped
    /// </summary>
    /// <returns> The last token returned by a call of Pop </returns>
    virtual const TokenClass& LastPopped() const
    {
        return m_tLastPopped;
    };

  protected:
    virtual TokenClass Pop_Internal()
    {
        return m_tLastPopped;
    }

    TokenClass m_tLastPopped{ TokenClass::TokenType::End };
};
