#pragma once

#include "FLIST.h"
#include "Queue.h"
#include "Stack.h"

bool find(flist::ptrNODE head, flist::ptrNODE& before, flist::ptrNODE& last);
bool task15(flist::FLIST& list);
std::size_t word_length(const std::string& word);
std::size_t task12(std::istream& input, QueueL& words);

template <class Stack>
bool balanced(const std::string& text, Stack& stack)
{
    bool correct{ true };
    std::size_t i{};

    while (i < text.size() && correct)
    {
        char symbol{ text[i] };
        if (symbol == '(' || symbol == '[' || symbol == '{')
            stack.push(symbol);
        else if (symbol == ')' || symbol == ']' || symbol == '}')
        {
            if (stack.empty())
                correct = false;
            else
            {
                char opening{ stack.top() };
                correct = (opening == '(' && symbol == ')')
                    || (opening == '[' && symbol == ']')
                    || (opening == '{' && symbol == '}');
                stack.pop();
            }
        }
        ++i;
    }

    correct = correct && stack.empty();
    return correct;
}
