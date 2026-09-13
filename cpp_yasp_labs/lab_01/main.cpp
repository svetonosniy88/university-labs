/*
    Лабораторная работа № 1.
    Задача связности.

    Условие:
    Имеется последовательность пар целых чисел p-q.
    Пара p-q интерпретируется как связь элемента p с элементом q.
    Если p связан с q, а q связан с r, то p считается связанным с r.

    Требуется написать программу для исключения лишних пар из набора:
    при получении очередной пары p-q она добавляется только в том случае,
    если из предыдущих пар ещё не следует, что p связан с q.

    Исполнитель:
    Кузнецов Дмитрий Олегович
    Группа 91
*/

#include "console_io.h"
#include "disjoint_set.h"
#include "element_array.h"

#include <iostream>

int main()
{
    ElementArray elements{
        nullptr,
        0,
        0
    };

    int choice{ -1 };

    while (choice != 0)
    {
        std::cout
            << "\n1. Set sequence\n"
            << "2. Add pair\n"
            << "3. Print groups\n"
            << "4. Clear data\n"
            << "0. Exit\n"
            << "Choice: ";

        choice = read_int();

        switch (choice)
        {
        case 1:
        {
            std::cout << "Enter number of pairs: ";
            int pairs_count{ read_int() };

            while (pairs_count <= 0)
            {
                std::cout
                    << "Number of pairs must be positive. Try again: ";
                pairs_count = read_int();
            }

            clear_data(elements);

            for (int index{}; index < pairs_count; ++index)
            {
                int p{};
                int q{};

                std::cout << "Enter pair " << index + 1 << ": ";
                read_pair(p, q);

                if (process_pair(elements, p, q))
                {
                    std::cout << "Pair accepted.\n";
                }
                else
                {
                    std::cout
                        << "Pair rejected: connection already exists.\n";
                }
            }

            break;
        }

        case 2:
        {
            int p{};
            int q{};

            std::cout << "Enter pair: ";
            read_pair(p, q);

            if (process_pair(elements, p, q))
            {
                std::cout << "Pair accepted.\n";
            }
            else
            {
                std::cout
                    << "Pair rejected: connection already exists.\n";
            }

            break;
        }

        case 3:
            print_groups(elements);
            break;

        case 4:
            clear_data(elements);
            std::cout << "Data cleared.\n";
            break;

        case 0:
            break;

        default:
            std::cout << "Unknown menu item.\n";
            break;
        }
    }

    clear_data(elements);
    return 0;
}
