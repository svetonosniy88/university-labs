#include "recursive_tasks.h"

flist::TInfo find_max(flist::ptrNODE ptr)
{
    flist::TInfo max{ ptr->info };
    if (ptr->next)
    {
        flist::TInfo max_tail{ find_max(ptr->next) };
        if (max_tail > max)
            max = max_tail;
    }
    return max;
}

void reverse_print(flist::ptrNODE ptr)
{
    if (ptr->next)
        reverse_print(ptr->next);
    std::cout << ptr->info << ' ';
}

bool replace_all(flist::ptrNODE ptr, flist::TInfo old_el, flist::TInfo new_el)
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

bool delete_first(flist::FLIST& list, flist::ptrNODE before, flist::TInfo elem)
{
    bool deleted{ false };
    if (before->next->info == elem)
    {
        list.erase_after(before);
        deleted = true;
    }
    else if (before->next->next)
        deleted = delete_first(list, before->next, elem);
    return deleted;
}

bool duplicate_first(flist::FLIST& list, flist::ptrNODE ptr, flist::TInfo elem)
{
    bool duplicated{ false };
    if (ptr->info == elem)
    {
        list.insert_after(ptr, elem);
        duplicated = true;
    }
    else if (ptr->next)
        duplicated = duplicate_first(list, ptr->next, elem);
    return duplicated;
}

bool duplicate_all(flist::FLIST& list, flist::ptrNODE ptr, flist::TInfo elem)
{
    bool duplicated{ false };
    if (ptr->next)
    {
        bool is_duplicated{ duplicate_all(list, ptr->next, elem) };
        duplicated = duplicated || is_duplicated;
    }
    if (ptr->info == elem)
    {
        list.insert_after(ptr, elem);
        duplicated = true;
    }
    return duplicated;
}

bool is_increasing(flist::ptrNODE ptr)
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
