#include "recursive_tasks.h"

#include <exception>
#include <stdexcept>

void demonstrate(flist::FLIST& list)
{
    std::cout << "Initial list: ";
    list.print();
    if (!list.empty())
    {
        std::cout << "Maximum: " << find_max(list.get_head()->next) << '\n';
        std::cout << "Reverse: ";
        reverse_print(list.get_head()->next);
        std::cout << '\n';

        std::cout << "\nReplace 1 with 2:\n";
        std::cout << std::boolalpha << "Changed: "
            << replace_all(list.get_head()->next, 1, 2) << '\n';
        list.print();

        std::cout << "\nDelete first 5:\n";
        std::cout << "Deleted: " << delete_first(list, list.get_head(), 5) << '\n';
        list.print();

        // Deleting the only element can leave the list empty.
        if (!list.empty())
        {
            std::cout << "\nDuplicate first 2:\n";
            std::cout << "Duplicated: "
                << duplicate_first(list, list.get_head()->next, 2) << '\n';
            list.print();
            std::cout << "\nDuplicate all 4:\n";
            std::cout << "Duplicated: "
                << duplicate_all(list, list.get_head()->next, 4) << '\n';
            list.print();
            std::cout << "\nStrictly increasing: "
                << is_increasing(list.get_head()->next) << '\n';
        }
    }
}

int main()
{
    int result{};
    try
    {
        std::ifstream file("data.txt");
        if (!file)
            throw std::runtime_error("Cannot open data.txt in the working directory.");
        flist::FLIST list;
        list.create_by_queue(file);
        demonstrate(list);
    }
    catch (const std::exception& error)
    {
        std::cerr << "Error: " << error.what() << '\n';
        result = 1;
    }
    return result;
}
