#include <cstdint>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

import lab_queue;
import lab_io;

// ЯСП, лабораторная № 3. Вариант 4B — шаблон очереди на указателях.
// Кузнецов Дмитрий Олегович, группа 91.

// Меню работает с выбранной очередью. Возврат к типам сохраняет её содержимое.
template<class X>
void run_menu(Queue<X>& queue, const char* type, const char* format)
{
    while (std::cin)
    {
        std::cout << "\nQueue: " << type << "\n1. Push  2. Peek  3. Pop  4. Size  5. Find\n"
                     "6. Print  7. Load from file  8. Save to file  0. Back\n";
        std::uint32_t choice{};
        if (!read_choice("Choice: ", choice, 0, 8) || choice == 0)
            return;
        try
        {
            switch (choice)
            {
            case 1:
            case 5:
            {
                X value{};
                if (!read_value_from_console(format, value))
                    return;
                if (choice == 1)
                {
                    queue.push(value);
                    std::cout << "Added.\n";
                    break;
                }
                std::uint32_t position{ queue.find(value) };
                if (position)
                    std::cout << "Position: " << position << '\n';
                else
                    std::cout << "Not found.\n";
                break;
            }
            case 2:
            {
                const X& first{ queue.peek() };
                std::cout << "Head: " << first << '\n';
                break;
            }
            case 3:
            {
                X first{ queue.pop() };
                std::cout << "Removed: " << first << '\n';
                break;
            }
            case 4: std::cout << "Size: " << queue.size() << '\n'; break;
            case 6:
                if (queue.size() == 0)
                    std::cout << "Queue is empty.\n";
                else
                    queue.print(std::cout);
                break;
            case 7:
                std::cout << "One record per line: " << format << ".\n";
                load_queue(queue);
                break;
            case 8: save_queue(queue); break;
            }
        }
        catch (const std::exception& error)
        {
            std::cout << "Error: " << error.what() << '\n';
        }
    }
}

int main()
{
#ifdef _WIN32
    ConsoleEncoding console;
#endif
    Queue<std::int32_t> numbers;
    Queue<std::string> strings;
    Queue<Student> students;
    try
    {
        while (std::cin)
        {
            std::uint32_t type{};
            if (!read_choice("\n1. Integers  2. Strings  3. Students  0. Exit: ", type, 0, 3) || type == 0)
                break;
            switch (type)
            {
            case 1: run_menu(numbers, "integers", "an integer from -2147483648 to 2147483647"); break;
            case 2: run_menu(strings, "strings", "a line of text"); break;
            case 3: run_menu(students, "students", "record number|full name|group"); break;
            }
        }
    }
    catch (const std::exception& error)
    {
        std::cerr << "Console error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
