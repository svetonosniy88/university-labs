#include "list.h"

#include <fstream>
#include <iostream>


TInfo find_max(ptrNODE ptr)
{
    TInfo max{ ptr->info };

    if (ptr->next)
    {
        TInfo max_tail{ find_max(ptr->next) };

        if (max_tail > max)
            max = max_tail;
    }

    return max;
}

void reverse_print(ptrNODE ptr)
{
    if (ptr->next)
        reverse_print(ptr->next);

    std::cout << ptr->info << ' ';
}

bool replace_all(ptrNODE ptr, TInfo old_el, TInfo new_el)
{
    bool changed{ false };

    if (ptr->info == old_el)
    {
        ptr->info = new_el;
        changed = true;
    }

    if (ptr->next)
    {
        bool is_changed{ replace_all(ptr->next, old_el, new_el) };
        changed = changed || is_changed;
    }

    return changed;
}

bool delete_first(ptrNODE& ptr, TInfo deleting_el)
{
    bool deleted{ false };

    if (ptr->info == deleting_el)
    {
        del_by_pointer(ptr);
        deleted = true;
    }
    else if (ptr->next)
        deleted = delete_first(ptr->next, deleting_el);

    return deleted;
}

bool duplicate_first(ptrNODE ptr, TInfo duplicating_el)
{
    bool duplicated{ false };

    if (ptr->info == duplicating_el)
    {
        add_after(ptr, duplicating_el);
        duplicated = true;
    }
    else if (ptr->next)
        duplicated = duplicate_first(ptr->next, duplicating_el);

    return duplicated;
}

bool duplicate_all(ptrNODE ptr, TInfo duplicating_el)
{
    bool duplicated{ false };

    if (ptr->next)
    {
        bool is_duplicated{ duplicate_all(ptr->next, duplicating_el) };
        duplicated = duplicated || is_duplicated;
    }

    if (ptr->info == duplicating_el)
    {
        add_after(ptr, duplicating_el);
        duplicated = true;
    }

    return duplicated;
}

bool is_increasing(ptrNODE ptr)
{
    bool ordered{ true };

    if (ptr->next)
    {
        if (ptr->info >= ptr->next->info)
            ordered = false;
        else
            ordered = is_increasing(ptr->next);
    }

    return ordered;
}

int main()
{
    TInfo values[]{ 3, 1, 5, 1, 4, 1 };

    ptrNODE head{};
    init(head);

    ptrNODE tail{ head };

    for (TInfo elem : values)
    {
        add_after(tail, elem);
        tail = tail->next;
    }

    std::cout << "Initial list: ";
    print(head);

    std::cout << "Maximum: " << find_max(head->next) << '\n';

    std::cout << "Reverse: ";
    reverse_print(head->next);
    std::cout << '\n';

    std::cout << "\nReplace 1 with 2:\n";
    std::cout << std::boolalpha << "Changed: " << replace_all(head->next, 1, 2) << '\n';
    print(head);

    std::cout << "\nDelete first 5:\n";
    std::cout << "Deleted: " << delete_first(head->next, 5) << '\n';
    print(head);

    std::cout << "\nDuplicate first 2:\n";
    std::cout << "Duplicated: " << duplicate_first(head->next, 2) << '\n';
    print(head);

    std::cout << "\nDuplicate all 4:\n";
    std::cout << "Duplicated: " << duplicate_all(head->next, 4) << '\n';
    print(head);

    std::cout << "\nStrictly increasing: " << is_increasing(head->next) << '\n';

    clear(head);

    return 0;
}