#include "list.h"

#include <functional>
#include <stdexcept>

void init(ptrNODE& head)
{
    head = nullptr;
}

bool empty(ptrNODE head)
{
    return head == nullptr;
}

void add_by_pointer(ptrNODE& ptr, TInfo elem)
{
    ptr = new NODE(elem, ptr);
}

void add_to_head(ptrNODE& head, TInfo elem)
{
    add_by_pointer(head, elem);
}

void add_after(ptrNODE& ptr, TInfo elem)
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

void del_from_head(ptrNODE& head)
{
    del_by_pointer(head);
}

void del_after(ptrNODE& ptr)
{
    if (ptr != nullptr)
        del_by_pointer(ptr->next);
}

void print(ptrNODE head, std::ostream& stream)
{
    if (empty(head))
        stream << "empty";
    else
    {
        ptrNODE ptr{head};
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
        del_from_head(head);
}

void create_by_stack(ptrNODE& head, std::ifstream& file)
{
    init(head);
    TInfo elem{};
    while (file >> elem)
        add_to_head(head, elem);
}

void create_by_queue(ptrNODE& head, std::ifstream& file)
{
    init(head);
    TInfo elem{};

    if (file >> elem)
    {
        add_to_head(head, elem);
        ptrNODE tail{head};
        while (file >> elem)
        {
            add_after(tail, elem);
            tail = tail->next;
        }
    }
}

ptrNODE create_by_order(std::ifstream& file)
{
    ptrNODE head{};
    init(head);
    TInfo elem{};

    std::function<ptrNODE(TInfo)> find_place = [&head](TInfo elem)
    {
        ptrNODE ptr{head};
        while (ptr->next && ptr->next->info < elem)
            ptr = ptr->next;
        return ptr;
    };

    try
    {
        while (file >> std::ws && !file.eof())
        {
            if (!(file >> elem))
                throw std::runtime_error("input.txt must contain only integers in the int range.");

            if (empty(head) || elem < head->info)
                add_to_head(head, elem);
            else
            {
                ptrNODE ptr{find_place(elem)};
                add_after(ptr, elem);
            }
        }

        if (file.bad())
            throw std::runtime_error("Failed to read input.txt.");
    }
    catch (...)
    {
        clear(head);
        throw;
    }

    return head;
}
