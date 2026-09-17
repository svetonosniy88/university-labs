#include <fstream>
#include <functional>
#include <iostream>

using TInfo = int;

struct NODE
{
    TInfo info;
    NODE* next;

    NODE(TInfo info, NODE* ptr) : info(info), next(ptr) {}

    ~NODE()
    {
        next = nullptr;
    }
};

using ptrNODE = NODE*;

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

void print(ptrNODE head, std::ostream& stream = std::cout)
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

    if (!(file >> elem))
        return;

    add_to_head(head, elem);
    ptrNODE tail{head};
    while (file >> elem)
    {
        add_after(tail, elem);
        tail = tail->next;
    }
}

ptrNODE create_by_order(std::ifstream& file)
{
    ptrNODE head{};
    init(head);
    TInfo elem{};

    if (!(file >> elem))
        return head;

    add_to_head(head, elem);
    std::function<ptrNODE(TInfo)> find_place = [&head](TInfo elem)
    {
        ptrNODE ptr{head};
        while (ptr->next && ptr->next->info < elem)
            ptr = ptr->next;
        return ptr;
    };

    while (file >> elem)
    {
        if (elem < head->info)
            add_to_head(head, elem);
        else
        {
            ptrNODE ptr{find_place(elem)};
            add_after(ptr, elem);
        }
    }
    return head;
}

void task1(ptrNODE head)
{
    ptrNODE ptr{head};
    while (ptr)
    {
        if (ptr->info % 2 != 0)
        {
            add_after(ptr, ptr->info);
            ptr = ptr->next;
        }
        ptr = ptr->next;
    }
}

int main()
{
    std::ifstream file("data.txt");
    if (!file)
    {
        std::cout << "File error\n";
        return 1;
    }

    ptrNODE head{};
    create_by_stack(head, file);

    std::cout << "List: ";
    print(head);

    task1(head);
    std::cout << "After duplicating odd elements: ";
    print(head);

    clear(head);
    return 0;
}
