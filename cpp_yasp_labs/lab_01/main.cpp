#include <iostream>

import lab_module;


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
            << "\n1. Set sequence from console\n"
            << "2. Load sequence from input.txt\n"
            << "3. Add pair\n"
            << "4. Print groups to console\n"
            << "5. Save groups to result.txt\n"
            << "6. Clear data\n"
            << "0. Exit\n"
            << "Choice: ";

        choice = read_int();

        switch (choice)
        {
        case 1:
        {
            int pairs_count{};

            std::cout << "Enter number of pairs: ";
            pairs_count = read_int();

            while (pairs_count <= 0)
            {
                std::cout
                    << "Number of pairs must be positive. Try again: ";

                pairs_count = read_int();
            }

            clear_data(elements);

            for (int i{}; i < pairs_count; ++i)
            {
                int p{};
                int q{};

                std::cout
                    << "Enter pair "
                    << i + 1
                    << ": ";

                read_pair(p, q);

                if (process_pair(elements, p, q))
                    std::cout << "Pair accepted.\n";
                else
                    std::cout
                    << "Pair rejected: connection already exists.\n";
            }

            break;
        }

        case 2:
            if (load_from_file(elements))
                std::cout << "Data loaded from input.txt.\n";
            else
                std::cout
                << "Failed to load input.txt. "
                << "Check the file and its contents.\n";
            break;

        case 3:
        {
            int p{};
            int q{};

            std::cout << "Enter pair: ";

            read_pair(p, q);

            if (process_pair(elements, p, q))
                std::cout << "Pair accepted.\n";
            else
                std::cout
                << "Pair rejected: connection already exists.\n";

            break;
        }

        case 4:
            print_groups(elements, std::cout);
            break;

        case 5:
            if (save_to_file(elements))
                std::cout << "Data saved to result.txt.\n";
            else
                std::cout << "Failed to create result.txt.\n";
            break;

        case 6:
            clear_data(elements);
            clear_result_file();
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