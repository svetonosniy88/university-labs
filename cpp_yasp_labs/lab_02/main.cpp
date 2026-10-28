#include <iostream>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <exception>

import lab_module;

/*
    Лабораторная работа № 2, вариант 4.
    Создать класс Fraction для обыкновенных дробей: числитель со знаком,
    беззнаковый знаменатель; +, -, *, /, степень, сравнения.
    Исполнитель: Кузнецов Дмитрий Олегович, группа 91.
*/

int main()
{
    Fraction a, b;
    char last_output[512]{};

    while (true)
    {
        std::cout
            << "\n1. Enter A and B from console\n"
            << "2. Load two fractions from file (one per line)\n"
            << "3. Show A and B\n"
            << "4. Calculate A + B, A - B, A * B or A / B\n"
            << "5. Raise A or B to a power\n"
            << "6. Compare A and B\n"
            << "7. Prefix or postfix increment\n"
            << "8. Save last successful report to file (overwrite)\n"
            << "0. Exit\n";

        try
        {
            std::uint32_t choice{};

            if (!read_choice("Choice: ", choice, 0, 8) || choice == 0)
                break;

            char output[512]{};
            char first[32]{}, second[32]{}, answer[32]{};

            a.to_string(first, sizeof(first));
            b.to_string(second, sizeof(second));

            switch (choice)
            {
            case 1:
                if (!read_from_console(a, b))
                    return 0;

                break;

            case 2:
            case 8:
            {
                bool saving{ choice == 8 };

                if (saving && last_output[0] == '\0')
                {
                    std::cout << "Show the fractions or perform an operation first.\n";
                    continue;
                }

                if (saving)
                    std::cout << "Saving the last successful report shown on screen.\n";
                else
                    std::cout << "Input: two fractions on separate lines, as integers, n/d or n d.\n";

                while (true)
                {
                    char name[512]{};

                    if (!read_filename(name, sizeof(name), saving))
                        return 0;

                    try
                    {
                        if (saving)
                            save_to_file(last_output, name);
                        else
                        {
                            std::cout << "Reading from " << name << '\n';
                            load_from_file(a, b, name);
                        }

                        break;
                    }
                    catch (const std::exception& error)
                    {
                        std::cout << "File error: " << error.what() << "\nEnter another file name.\n";
                    }
                }

                if (saving)
                    continue;

                break;
            }

            case 3:
                break;

            case 4:
            {
                std::uint32_t operation{};

                if (!read_choice("1. Add  2. Subtract  3. Multiply  4. Divide: ", operation, 1, 4))
                    return 0;

                Fraction result;
                const char signs[]{ '+', '-', '*', '/' };

                switch (operation)
                {
                case 1:
                    result = a + b;
                    break;

                case 2:
                    result = a - b;
                    break;

                case 3:
                    result = a * b;
                    break;

                case 4:
                    result = a / b;
                    break;
                }

                result.to_string(answer, sizeof(answer));

                std::snprintf(
                    output, sizeof(output),
                    "(%s) %c (%s) = %s\n",
                    first, signs[operation - 1], second, answer
                );

                break;
            }

            case 5:
            {
                std::uint32_t object{};
                Fraction exponent;

                if (!read_choice("1. A  2. B: ", object, 1, 2))
                    return 0;

                if (!read_fraction_from_console("Exponent (integer, p/q or p q): ", exponent))
                    return 0;

                const Fraction& selected{ object == 1 ? a : b };
                Fraction result{ selected.power(exponent) };

                selected.to_string(first, sizeof(first));
                exponent.to_string(second, sizeof(second));
                result.to_string(answer, sizeof(answer));

                std::snprintf(
                    output, sizeof(output),
                    "(%s) ^ (%s) = %s\n",
                    first, second, answer
                );

                break;
            }

            case 6:
                std::snprintf(
                    output, sizeof(output),
                    "A = %s\nB = %s\n\n"
                    "A == B: %s\nA != B: %s\n"
                    "A <  B: %s\nA <= B: %s\n"
                    "A >  B: %s\nA >= B: %s\n",
                    first, second,
                    a == b ? "true" : "false",
                    a != b ? "true" : "false",
                    a < b ? "true" : "false",
                    a <= b ? "true" : "false",
                    a > b ? "true" : "false",
                    a >= b ? "true" : "false"
                );

                break;

            case 7:
            {
                std::uint32_t object{}, kind{};

                if (!read_choice("1. A  2. B: ", object, 1, 2))
                    return 0;

                if (!read_choice("1. Prefix ++x  2. Postfix x++: ", kind, 1, 2))
                    return 0;

                Fraction& selected{ object == 1 ? a : b };
                char before[32]{};
                selected.to_string(before, sizeof(before));

                Fraction returned;

                if (kind == 1)
                    returned = ++selected;
                else
                    returned = selected++;

                returned.to_string(first, sizeof(first));
                selected.to_string(answer, sizeof(answer));

                std::snprintf(
                    output, sizeof(output),
                    "%c before: %s\nOperation: %s\nReturned: %s\n%c after: %s\n",
                    object == 1 ? 'A' : 'B', before,
                    kind == 1 ? "prefix ++x" : "postfix x++",
                    first, object == 1 ? 'A' : 'B', answer
                );

                break;
            }

            }

            if (choice >= 1 && choice <= 3)
            {
                a.to_string(first, sizeof(first));
                b.to_string(second, sizeof(second));

                std::snprintf(output, sizeof(output), "A = %s\nB = %s\n", first, second);
            }

            std::memcpy(last_output, output, sizeof(last_output));
            std::cout << last_output;
        }
        catch (const std::exception& error)
        {
            std::cout << "Error: " << error.what() << '\n';

            if (std::cin.bad())
                return 1;
        }
    }

    return 0;
}
