#pragma once

#include <deque>
#include <ostream>

//////////////////////////////////////////////////////////////
class Stack
{
public:
    void Clear();
    int GetSize() const;
    bool Empty() const;
    void Push(int val);
    int Pop();
    void Roll(int depth, int rotations);

    //////////////////////////////////////////////////////////////
    friend std::ostream& operator<<(std::ostream& os, const Stack& pt)
    {
        for (auto it = pt.m_stack.rbegin(); it != pt.m_stack.rend(); it++)
        {
            os << *it << " ";
        }
        return os;
    }

    const std::deque<int>& GetStack() const;
    void DisplayStack() const;

private:
    std::deque<int> m_stack;
};
