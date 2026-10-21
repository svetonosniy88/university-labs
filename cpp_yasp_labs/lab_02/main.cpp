#include <iostream>
#include <cstdint>
#include <exception>

import lab_module;

/*
    Лабораторная работа № 2, вариант 4.
    Создать класс Fraction для обыкновенных дробей: числитель со знаком,
    беззнаковый знаменатель; +, -, *, /, натуральная степень, сравнения.
    Исполнитель: Кузнецов Дмитрий Олегович, группа 91.
*/

int main()
{
    Fraction a, b, result;
    bool has_result{ false };
    std::uint32_t choice{};

    while (true)
    {
        std::cout
            << "\n1. Enter A and B from console\n"
            << "2. Load A and B from file\n"
            << "3. Show A, B and last result\n"
            << "4. Calculate A + B, A - B, A * B or A / B\n"
            << "5. Natural power\n"
            << "6. Compare A and B\n"
            << "7. Prefix or postfix increment\n"
            << "8. Save to file (overwrite)\n"
            << "0. Exit\n";

        try
        {
            if (!read_number("Choice: ", choice) || choice == 0)
                break;

            switch (choice)
            {
            case 1:
                if (!read_from_console(a, b))
                    return 0;

                has_result = false;
                std::cout << "A and B updated.\n";

                break;

            case 2:
            {
                char name[512]{};

                if (!read_filename(name, sizeof(name)))
                    return 0;

                load_from_file(a, b, name);
                has_result = false;

                std::cout << "A and B loaded.\n";

                break;
            }

            case 3:
                print_fraction("A = ", a);
                print_fraction("B = ", b);

                if (has_result)
                    print_fraction("Last result = ", result);

                break;

            case 4:
            {
                std::uint32_t operation{};

                if (!read_number("1. Add  2. Subtract  3. Multiply  4. Divide: ", operation))
                    return 0;

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

                default:
                    std::cout << "Unknown operation.\n";
                    continue;
                }

                has_result = true;
                print_fraction("Result = ", result);

                break;
            }

            case 5:
            case 7:
            {
                std::uint32_t object{}, parameter{};

                if (!read_number("1. A  2. B: ", object))
                    return 0;

                if (object != 1 && object != 2)
                {
                    std::cout << "Choose A or B.\n";
                    break;
                }

                Fraction& selected{ object == 1 ? a : b };

                if (choice == 5)
                {
                    if (!read_number("Natural exponent (at least 1): ", parameter))
                        return 0;

                    result = selected.power(parameter);
                }
                else
                {
                    if (!read_number("1. Prefix ++x  2. Postfix x++: ", parameter))
                        return 0;

                    if (parameter == 1)
                        result = ++selected;
                    else if (parameter == 2)
                        result = selected++;
                    else
                    {
                        std::cout << "Unknown increment type.\n";
                        break;
                    }

                    print_fraction("Object after increment = ", selected);
                }

                has_result = true;
                print_fraction("Result = ", result);

                break;
            }

            case 6:
                std::cout << std::boolalpha
                    << "A == B: " << (a == b) << '\n'
                    << "A != B: " << (a != b) << '\n'
                    << "A <  B: " << (a < b) << '\n'
                    << "A <= B: " << (a <= b) << '\n'
                    << "A >  B: " << (a > b) << '\n'
                    << "A >= B: " << (a >= b) << '\n';

                break;

            case 8:
            {
                std::uint32_t contents{};

                if (!read_number("1. Save A and B  2. Save last result: ", contents))
                    return 0;

                if (contents != 1 && contents != 2)
                {
                    std::cout << "Unknown file contents.\n";
                    break;
                }

                if (contents == 2 && !has_result)
                {
                    std::cout << "Calculate a result first.\n";
                    break;
                }

                char name[512]{};

                if (!read_filename(name, sizeof(name)))
                    return 0;

                if (contents == 1)
                    save_to_file(a, b, name);
                else
                    save_to_file(result, result, name, true);

                std::cout << "File saved (previous contents replaced).\n";

                break;
            }

            default:
                std::cout << "Unknown menu item.\n";
                break;
            }
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
