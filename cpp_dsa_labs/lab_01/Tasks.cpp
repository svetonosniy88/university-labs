#include "Tasks.h"

bool find(flist::ptrNODE head, flist::ptrNODE& before, flist::ptrNODE& last)
{
    before = nullptr;
    last = nullptr;
    flist::ptrNODE ptr{ head };
    bool found{ false };
    while (ptr->next && !found)
    {
        flist::ptrNODE first{ ptr->next };
        if (first->info % 2 != 0 && first->next && first->next->info % 2 != 0)
        {
            before = ptr;
            last = first->next;
            while (last->next && last->next->info % 2 != 0)
                last = last->next;
            found = true;
        }
        else
            ptr = first;
    }
    return found;
}

bool task15(flist::FLIST& list)
{
    flist::ptrNODE before{};
    flist::ptrNODE last{};
    bool found{ find(list.get_head(), before, last) };
    if (found && last->next)
    {
        flist::ptrNODE tail{ last };
        while (tail->next)
            tail = tail->next;
        list.switch_fragment(tail, before, last);
    }
    return found;
}

std::size_t word_length(const std::string& word)
{
    std::size_t length{};
    for (unsigned char symbol : word)
    {
        if ((symbol & 0xC0) != 0x80)
            ++length;
    }
    return length;
}

std::size_t task12(std::istream& input, QueueL& words)
{
    while (!words.empty())
        words.pop();

    std::size_t maximum{};
    std::string word;
    bool first{ true };

    while (input >> word)
    {
        if (first && word.compare(0, 3, "\xEF\xBB\xBF") == 0)
            word.erase(0, 3);
        first = false;

        std::size_t length{ word_length(word) };
        if (length > maximum)
        {
            while (!words.empty())
                words.pop();
            maximum = length;
        }
        if (length == maximum && length != 0)
            words.push(word);
    }
    return maximum;
}
