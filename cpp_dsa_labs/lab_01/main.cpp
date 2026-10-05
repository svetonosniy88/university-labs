#include "list.h"

#include <exception>
#include <stdexcept>


bool task15(ptrNODE& head)
{
    ptrNODE prev{ nullptr };
    ptrNODE ptr{ head };

    bool found{ false };

    // Ищем начало первой группы как минимум из двух нечётных элементов
    while (ptr != nullptr && !found)
    {
        // Пропускаем чётные элементы
        while (ptr != nullptr && ptr->info % 2 == 0)
        {
            prev = ptr;
            ptr = ptr->next;
        }

        // Если группа нашлась до конца списка
        if (ptr != nullptr)
        {
            // Группа существует только если следующий элемент тоже нечётный
            if (ptr->next != nullptr && ptr->next->info % 2 != 0)
            {
                found = true;
            }
            else
            {
                // Текущий нечётный элемент одиночный — пропускаем его
                prev = ptr;
                ptr = ptr->next;
            }
        }
    }

    if (found)
    {
        // ptr указывает на первый элемент найденной нечётной группы
        ptrNODE first{ ptr };

        // Ищем последний элемент группы
        while (ptr->next != nullptr && ptr->next->info % 2 != 0)
            ptr = ptr->next;

        ptrNODE last{ ptr };
        ptrNODE after{ last->next };

        // Переносим группу, если она ещё не находится в конце списка
        if (after != nullptr)
        {
            // Отсоединяем группу
            if (prev != nullptr)
                prev->next = after;
            else
                head = after;

            last->next = nullptr;

            // Ищем конец оставшегося списка
            ptrNODE tail{ after };
            while (tail->next != nullptr)
                tail = tail->next;

            // Присоединяем найденную группу в конец
            tail->next = first;
        }
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

        bool found{ task15(head) };

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