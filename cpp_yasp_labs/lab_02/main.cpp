#include <iostream>
#include <cstdint>
#include <cstdio>
#include <exception>

import lab_module;
import lab_io;

/*
 * Лабораторная работа № 2, вариант 4.
 * Класс Fraction: обыкновенные дроби; +, -, *, /, рациональная степень,
 * сравнения и инкременты; ввод и вывод в консоль и файл.
 * Кузнецов Дмитрий Олегович, группа 91.
 */

// Организует меню, вызовы операций с дробями, вывод отчётов и подтверждение сохранения.
int main()
{
    Fraction a, b;

    while (true)
    {
        std::cout
            << "\n1. Enter A and B from console\n"
            << "2. Load two fractions from file (one per line)\n"
            << "3. Show A and B\n"
            << "4. Add, subtract, multiply or divide\n"
            << "5. Raise A or B to a rational power\n"
            << "6. Compare A and B\n"
            << "7. Prefix or postfix increment\n"
            << "0. Exit\n";

        try
        {
            std::uint32_t choice{};
            if (!read_choice("Choice: ", choice, 0, 7) || choice == 0)
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
                if (!load_pair(a, b))
                {
                    if (!std::cin)
                        return 0;
                    continue;
                }
                break;

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
                case 1: result = a + b; break;
                case 2: result = a - b; break;
                case 3: result = a * b; break;
                case 4: result = a / b; break;
                }

                result.to_string(answer, sizeof(answer));
                std::snprintf(output, sizeof(output), "(%s) %c (%s) = %s\n", first, signs[operation - 1], second, answer);
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
                std::snprintf(output, sizeof(output), "(%s) ^ (%s) = %s\n", first, second, answer);
                break;
            }

            case 6:
                std::snprintf(output, sizeof(output),
                    "A = %s\nB = %s\n\n"
                    "A == B: %s\nA != B: %s\n"
                    "A <  B: %s\nA <= B: %s\n"
                    "A >  B: %s\nA >= B: %s\n",
                    first, second,
                    a == b ? "true" : "false", a != b ? "true" : "false",
                    a < b ? "true" : "false", a <= b ? "true" : "false",
                    a > b ? "true" : "false", a >= b ? "true" : "false");
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
                std::snprintf(output, sizeof(output),
                    "%c before: %s\nOperation: %s\nReturned: %s\n%c after: %s\n",
                    object == 1 ? 'A' : 'B', before,
                    kind == 1 ? "prefix ++x" : "postfix x++",
                    first, object == 1 ? 'A' : 'B', answer);
                break;
            }
            }

            if (choice >= 1 && choice <= 3)
            {
                a.to_string(first, sizeof(first));
                b.to_string(second, sizeof(second));
                std::snprintf(output, sizeof(output), "A = %s\nB = %s\n", first, second);
            }

            // После успешного действия показываем отчёт и предлагаем сохранить именно этот текст.
            std::cout << output;
            std::uint32_t save{};
            if (!read_choice("Save this report? 1. Yes  0. No: ", save, 0, 1))
                return 0;
            if (save == 0)
                continue;

            save_report(output);
            if (!std::cin)
                return 0;
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
