#include "list.h"

#include <stdexcept>

namespace
{
    bool read_value(std::ifstream& file, TInfo& elem)
    {
        file >> std::ws;

        if (file.bad() || (file.fail() && !file.eof()))
            throw std::runtime_error("Failed to read input.txt.");

        bool found{!file.eof()};
        if (found && !(file >> elem))
            throw std::runtime_error("input.txt must contain only integers in the int range.");

        return found;
    }
}

void init(ptrNODE& head)
{
    head = new NODE(0, nullptr);
}

bool empty(ptrNODE head)
{
    return head->next == nullptr;
}

void add_by_pointer(ptrNODE& ptr, TInfo elem)
{
    ptr = new NODE(elem, ptr);
}

void add_to_head(ptrNODE head, TInfo elem)
{
    add_after(head, elem);
}

void add_after(ptrNODE ptr, TInfo elem)
{
    if (ptr != nullptr)
        add_by_pointer(ptr->next, elem);
}

void del_by_pointer(ptrNODE& ptr)
{
    if (ptr != nullptr)
    {
        ptrNODE tmp{ptr};
        ptr = tmp->next;
        delete tmp;
    }
}

void del_from_head(ptrNODE head)
{
    del_after(head);
}

void del_after(ptrNODE ptr)
{
    if (ptr != nullptr)
        del_by_pointer(ptr->next);
}

void switch_fragment(ptrNODE before, ptrNODE last, ptrNODE place)
{
    ptrNODE first{before->next};
    before->next = last->next;
    last->next = place->next;
    place->next = first;
}

void print(ptrNODE head, std::ostream& stream)
{
    if (empty(head))
        stream << "empty";
    else
    {
        ptrNODE ptr{head->next};
        while (ptr)
        {
            stream << ptr->info << ' ';
            ptr = ptr->next;
        }
    }
    stream << '\n';
}

void clear(ptrNODE& head)
{
    while (head)
        del_by_pointer(head);
}

void create_by_stack(ptrNODE& head, std::ifstream& file)
{
    clear(head);
    init(head);
    TInfo elem{};

    try
    {
        while (read_value(file, elem))
            add_to_head(head, elem);
    }
    catch (...)
    {
        clear(head);
        throw;
    }
}

void create_by_queue(ptrNODE& head, std::ifstream& file)
{
    clear(head);
    init(head);
    TInfo elem{};
    ptrNODE tail{head};

    try
    {
        while (read_value(file, elem))
        {
            add_after(tail, elem);
            tail = tail->next;
        }
    }
    catch (...)
    {
        clear(head);
        throw;
    }
}

ptrNODE create_by_order(std::ifstream& file)
{
    ptrNODE head{};
    init(head);
    TInfo elem{};

    try
    {
        while (read_value(file, elem))
        {
            ptrNODE ptr{head};
            while (ptr->next != nullptr && ptr->next->info < elem)
                ptr = ptr->next;
            add_after(ptr, elem);
        }
    }
    catch (...)
    {
        clear(head);
        throw;
    }

    return head;
}
