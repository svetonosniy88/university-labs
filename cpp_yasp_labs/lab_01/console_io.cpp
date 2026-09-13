#include "console_io.h"

#include "disjoint_set.h"

#include <iostream>
#include <limits>

namespace
{
    // Удаляет из входного потока остаток текущей строки,
    // чтобы ошибочные символы не попали в следующий ввод.
    void discard_input_line()
    {
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );
    }
}

int read_int()
{
    int value{};

    // Если извлечь int не удалось, флаг ошибки сбрасывается,
    // некорректная строка удаляется и ввод повторяется.
    while (!(std::cin >> value))
    {
        std::cout << "Invalid input. Enter an integer: ";
        std::cin.clear();
        discard_input_line();
    }

    // После успешного чтения также отбрасывается хвост строки:
    // один вызов read_int соответствует ровно одной строке ввода.
    discard_input_line();
    return value;
}

void read_pair(int& p, int& q)
{
    // Оба числа считываются одной операцией.
    // Частично введённая или некорректная пара отвергается целиком.
    while (!(std::cin >> p >> q))
    {
        std::cout << "Invalid pair. Enter two integers: ";
        std::cin.clear();
        discard_input_line();
    }

    discard_input_line();
}

void print_groups(const ElementArray& elements)
{
    if (elements.count == 0)
    {
        std::cout << "No data.\n";
        return;
    }

    // Внешний цикл находит корни всех компонент.
    for (int root{}; root < elements.count; ++root)
    {
        if (elements.data[root].parent != root)
        {
            continue;
        }

        std::cout << "{ ";

        // Внутренний цикл выводит все элементы,
        // корень которых совпадает с текущим корнем.
        for (int index{}; index < elements.count; ++index)
        {
            if (find_root(elements, index) == root)
            {
                std::cout << elements.data[index].value << ' ';
            }
        }

        std::cout << "}\n";
    }
}
