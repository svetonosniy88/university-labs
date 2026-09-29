#include "list.h"

#include <exception>
#include <stdexcept>


bool task15(ptrNODE& head)
{
    ptrNODE prev{nullptr};
    ptrNODE ptr{head};
    //Ищем первый нечетный
    while (ptr && ptr->info % 2 == 0)
    {
        prev = ptr;
        ptr = ptr->next;
    }
    
    bool found{ptr != nullptr};
    
    if (found)
    {
        //Проходим по группе нечетных
        ptrNODE first{ptr};
        while (ptr->next && ptr->next->info % 2 != 0)
            ptr = ptr->next;

        ptrNODE last{ptr};
        ptrNODE after{last->next};

        //Выполняем по необходиомсти перенос группы в конец списка
        if (after != nullptr) //если сразу после группы нуллптр то перенос не требуется
        {
            //Отсоединение группы от основного списка
            if (prev != nullptr)
                prev->next = after;
            else
                head = after;
            last->next = nullptr;

            //Проход по списку до конца и привязка отвязанной группы нечётных
            ptrNODE tail{after};
            while (tail->next)
                tail = tail->next;
            tail->next = first;
        }
    }

    return found;
}

int main()
{
    ptrNODE head{};
    int result{0};

    try
    {
        std::ifstream input("input.txt");
        if (!input)
            throw std::runtime_error("Cannot open input.txt in the working directory.");

        head = create_by_order(input);
        std::cout << "Sorted list: ";
        print(head);

        bool found{task15(head)};
        std::ofstream output("output.txt");
        if (!output)
            throw std::runtime_error("Cannot open output.txt for writing.");

        print(head, output);
        if (!found)
            output << "No odd numbers in the list.\n";

        output.close();
        if (!output)
            throw std::runtime_error("Failed to write output.txt.");

        std::cout << "Result: ";
        print(head);
        if (!found)
            std::cout << "No odd numbers in the list.\n";
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
