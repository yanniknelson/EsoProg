#pragma once

#include <deque>
#include <vector>

template <typename TokenClass>
class ITokeniser
{
  public:
    TokenClass Pop()
    {
        TokenClass ret = Peek();
        Consume();
        return ret;
    }

    TokenClass Peek()
    {
        if (m_buff.empty())
        {
            m_buff.push_back(Pop_Internal());
        }
        return m_buff.front();
    }

    std::vector<TokenClass> Peek(const size_t depth)
    {
        std::vector<TokenClass> ret;
        ret.reserve(depth);
        for (size_t currDepth = 0; currDepth < depth; currDepth++)
        {
            if (currDepth >= m_buff.size())
            {
                m_buff.push_back(Pop_Internal());
            }
            ret.push_back(m_buff.at(currDepth));
        }
        return ret;
    };

    void Consume(const int depth = 1)
    {
        for (size_t currDepth = 0; currDepth < depth; currDepth++)
        {
            if (!m_buff.empty())
            {
                m_buff.pop_front();
            }
            else
            {
                Pop_Internal();
            }
        }
    }

    void Reset()
    {
        m_buff.clear();
        ResetImplementation();
    }

    virtual void ResetImplementation() = 0;

  protected:
    virtual TokenClass Pop_Internal()
    {
        return TokenClass::TokenType::End;
    }

    std::deque<TokenClass> m_buff;
};
