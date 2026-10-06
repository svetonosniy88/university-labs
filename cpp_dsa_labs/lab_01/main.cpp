#include "list.h"

#include <exception>
#include <stdexcept>


bool find(ptrNODE head, ptrNODE& before, ptrNODE& last)
{
    before = nullptr;
    last = nullptr;
    ptrNODE ptr{head};
    bool found{false};

    while (ptr->next && !found)
    {
        ptrNODE first{ptr->next};
        if (first->info % 2 != 0 && 
            first->next != nullptr && 
            first->next->info % 2 != 0)
        {
            before = ptr;
            last = first->next;
            while (last->next != nullptr && last->next->info % 2 != 0)
                last = last->next;
            found = true;
        }
        else
            ptr = first;
    }

    return found;
}

bool task15(ptrNODE head)
{
    ptrNODE before{};
    ptrNODE last{};
    bool found{find(head, before, last)};

    if (found && last->next)
    {
        ptrNODE tail{ last };
        while (tail->next)
            tail = tail->next;

            switch_fragment(before, last, tail);
    }
    return found;
}


int main()
{
    ptrNODE head{};
    int result{ 0 };

    try
    {
        std::ifstream input("input.txt");
        if (!input)
            throw std::runtime_error(
                "Cannot open input.txt in the working directory."
            );

        head = create_by_order(input);

        std::cout << "Sorted list: ";
        print(head);

        

        bool found{task15(head)};

        std::ofstream output("output.txt");
        if (!output)
            throw std::runtime_error(
                "Cannot open output.txt for writing."
            );

        print(head, output);

        if (!found)
            output << "No group of at least two odd numbers in the list.\n";

        output.close();

        if (!output)
            throw std::runtime_error(
                "Failed to write output.txt."
            );

        std::cout << "Result: ";
        print(head);

        if (!found)
            std::cout
            << "No group of at least two odd numbers in the list.\n";

        std::cout << "Result saved to output.txt.\n";
    }
    catch (const std::exception& error)
    {
        std::cerr << "Error: " << error.what() << '\n';
        result = 1;
    }

    clear(head);
    return result;
}
